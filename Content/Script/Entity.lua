require "UnLua"
local LuaHotFixManager=require "luahotmanager"

local Entity=Class()

function Entity:ReceiveBeginPlay()
	self.Overridden.ReceiveBeginPlay(self)
	LuaHotFixManager.Init()
end

function Entity:TestString()
 	print("PString")
end

function Entity:PrintString()
	print("this is lua")
end

function Entity:UpdateLuaFile()
	print("更新")
	LuaHotFixManager.Update()
end

function Entity:LuaReload()
	print("FixLua")
	UE4.OnModuleHotfixed("Entity")
end

return Entity