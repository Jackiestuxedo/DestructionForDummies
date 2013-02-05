-- These need to go in their own file so DestroDummies gets declared
-- before anything else

-- Our base array
DestroDummies = {}

-- Destruction for Dummies variables NOT SAVED
DestroDummies.timeSinceLastUpdate = 0;
DestroDummies.beta = false;		-- Set to true to enable beta functions


-- Array used for translating from English into the local language
-- This is populated in LocaleLocal.lua
DestroDummies.string = {};


DestroDummies.eventFrame = CreateFrame("Frame");
DestroDummies.eventFrame:SetScript("OnEvent", function(this, event, ...)
	DestroDummies.events[event](...)
end);
DestroDummies.eventFrame:RegisterEvent("ADDON_LOADED");
DestroDummies.events = {};


-- storage for debuff cache
DestroDummies.debuffByAny = {};
DestroDummies.debuffByMe = {};
for i = 1,40 do
	DestroDummies.debuffByAny[i] = {};
	DestroDummies.debuffByMe[i] = {};
end


-- for linking together the different classes' rules
DestroDummies.lastClassVal = 0;

-- for mutex's
DestroDummies.mutex = {};
DestroDummies.lastMutexIndex = 0;

-- Action registries
DestroDummies.registry = {};
DestroDummies.registry.preCombat = {};
DestroDummies.registry.postSpell = {};
DestroDummies.registry.spellSent = {};
DestroDummies.registry.takeDamage = {};
DestroDummies.registry.takeHeal = {};
DestroDummies.registry.decideCombat = {};
DestroDummies.registry.decideNoCombat = {};


-- Cache some stuff for faster reference
DestroDummies.mainhandSlotNum = GetInventorySlotInfo("MainHandSlot");
DestroDummies.offhandSlotNum = GetInventorySlotInfo("SecondaryHandSlot");

-- Rogues can chant (poison) their thrown weapons
local _, cretinClass = UnitClass("player");
DestroDummies.canChantRanged = (cretinClass == "ROGUE");
