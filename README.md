# UE4-运行时热更新之Lua(一)

​	运行时热更,`Lua`方面测试了一下,应该基本可以使用了,`UE`内部的资源,现在也可以运行热更一部分,但是限制比较大,等基本完善的差不多了,再拿出来讨论一下,这里先说`Lua`部分

​	之前的`lua`更新如果想要在运行时热更新,可以`reload package`,但是这样会造成变量值的重置,所以这里就想要一种办法,在不重置变量值的情况下,只更新对应的逻辑代码.

​	所以,在[UnLua]([debugable-unlua](https://github.com/hxhb/debugable-unlua))和[lua_hotupdate](https://github.com/asqbtcupid/lua_hotupdate),基础上改了一部分代码,目前测试,基本可以使用了.

​	测试项目:[https://github.com/whoissunshijia/LuaHotReload](https://github.com/whoissunshijia/LuaHotReload)

## 修改部分

​	关于一些使用约定可以查看[http://asqbtcupid.github.io/hotupdate-manul/](http://asqbtcupid.github.io/hotupdate-manul/),这里就不细说了.这里大概说一下修改的地方:

+ `lua_hotupdate`修改了`luahotmanager`和`luahotupdate_lua53`,内部增加了一些更新的辅助表和函数,修改了文件的操作,目前在PC和安卓测试都可以使用

+ `UnLua`修改了`OnModuleHotfixed`函数,这里面说明的是:

  + `lua_hotupdate`每次更新可以获取`Lua`文件更新了哪些新的函数,但是这些函数并没有真正的插入到当前`lua`文件的`package`表内,所以在这里拿到所有更新函数的名字和地址,所以这里先暂时,把这些函数都`rawset(package.loaded[Name],ElementName,Element)`到对应的`Lua`文件表内.再通过`Unlua`的`OnModuleHotfixed`来找到`Lua`文件对应`UClass`内,去查找那些函数需要绑定,那些需要删除,这里需要讲一下函数的分类:

  + 第一类,纯粹的`Lua`函数,没有在蓝图或者`c++`内实现,这类函数,虽然刚刚插入到了表内,我们可以在`OnModuleHotfixed`内分析出来,等到热更结束,就从表内删除,不会影响到别的操作:

    ```
    for (auto Fun : NoNativeFunction)
    {
    	lua_getglobal(L, "rawset");    
    	lua_getglobal(L, "package"); 
    	lua_getfield(L, -1, "loaded");  
    	lua_getfield(L, -1, ModuleName.Get());   
    	lua_remove(L, -2);
    	lua_remove(L, -2);
    	lua_pushstring(L, TCHAR_TO_ANSI(*(Fun.ToString())));
    	lua_pushstring(L, NULL);			
    	lua_call(L, 3, 0);
    }
    ```

  + 第二类,蓝图定义的函数,在`Lua`覆写,针对这类函数,如果这次更新,新的`Lua`文件内还有这个函数,则不用管理,直接`OverrideFunction(*Func, Class, LuaFuncName);`,绑定到新的函数上,如果没有,.则代表这个函数不再使用,那么在函数内

    ```
    if (i)
    {
    	UFunction* NewFunc = DuplicateUFunction(i, Class, LuaFuncName);
    	RemoveUFunction(*Func, Class);
    	GReflectionRegistry.RemoveOverriddenFunction(*Func);
    	CachedNatives.Remove(*Func);
    	Class->AddFunctionToFunctionMap(NewFunc, LuaFuncName);
    	NoNativeFunction.Add(LuaFuncName);					
    }
    ```

    会先找到之前蓝图的`UFunction`,把这个`UFunction`复制出来,把当前绑定的`UFunciton`删除,把复制出来的蓝图`UFunction`绑定上去,这样就不再调用`Lua`的函数,而是使用蓝图的函数

  + 第三类,`c++`定义的`UFunction`,未在蓝图覆写,和上面类似,如果此次更新,在`Lua`文件内,还有这个函数,那么不用管理,直接绑定新的,如果不再使用的话,就在函数:

    ```
    else
    {	
    	UFunction* l=GReflectionRegistry.FindOverriddenFunctionByName((*Func)->GetFName());
    	RemoveUFunction(l, Class);
    	GReflectionRegistry.RemoveOverriddenFunction(l);
    	CachedNatives.Remove(l);
    	NoNativeFunction.Add(LuaFuncName);
    }		
    ```

    这里就直接把在`Lua`绑定的函数删除.

## 使用

​	在测试项目中,`hotupdatelist.lua`记录更新的`Lua`文件名称

+ 按键1调用一个在蓝图定义的函数,没有在`c++`定义,可以在运行的时候动态修改`function Entity:PrintString()`.

+ 按键2调用了更新`Lua`文件的函数,这个步骤是更新`Lua`文件内的函数,可以修改逻辑,但是如果这个函数是`UFunction`的,而且是删除或者增加,那么这一步不会更新绑定到最新的.

+ 按键8调用`OnModuleHotfixed`,这个步骤可以把上一步,删除或者新增的在`Lua`文件中的`UFunction`更新.进而再次调用就会使用最新的函数逻辑.

+ 按键0调用了`Mount File`,这个函数是给打包出来的测试项目用的,`Mount`到最新的`Lua`文件,进而更新逻辑.

  一般的流程就是,修改代码->`Mount`新的`Lua`文件->`LuaHotFixManager.Init()`->`UE4.OnModuleHotfixed("lua file name")`

