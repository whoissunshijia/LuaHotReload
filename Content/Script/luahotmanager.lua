require "UnLua"

require "hotupdatelist"

local HU = require "luahotupdate_lua53"

local OldFileFunction={}
local NewFileFunction={}
local NeedAddFunction={}
local NeedRemoveFunction={}
local LuaHotFixManager={}

function LuaHotFixManager.Init()
	local ScriptPath=UE4.FPaths.ProjectScriptDir()
	HU.Init("hotupdatelist", {ScriptPath})
end

--获取当前状态下的lua文件中的函数
function LuaHotFixManager.GetFillFunction(FunctionTable)
	for LuaName,Path in pairs(HU.HUMap) do
		Module = require(LuaName)
		local AllFunction={}
		for fname,obj in pairs(Module) do  	 
    		if(type(obj)=="function") then
    			AllFunction[fname]=obj
    		end
		end
		FunctionTable[LuaName]=AllFunction
	end
end

function LuaHotFixManager.Update()
	HU.Update()
	for x,y in pairs(HU.NewFunctionTable) do
		for i,l in pairs(y) do
			print(i,l)
			UE4.AddNewUpdateLuaFunctiones(x,i)
		end
	end
	HU.NewFunctionTable={}
end

return LuaHotFixManager