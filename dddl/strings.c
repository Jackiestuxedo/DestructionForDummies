/*
 *  Known strings (for spells, etc)
 *
 *  This is pretty clever, if I say so myself.  We are going to learn the 
 *  local name for every spell by just knowing the mapping between the name
 *  in English and the spell id.  With this mapping, we can query the server
 *  to tell us the local name.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strings.h"

// map between names and spell numbers
static struct spellInfo {
    char *name;
    int number;
} knownThings[] = {
    // things that are not spells--must be translated in the actual
    // Locale files
    { "Undead", -1 },
    { "Demon", -1 },
}, knownItems[] = {
    { "Glyph of Exorcism",	41103 },
    { "Glyph of Ferocious Bite",67600 },
    { "Glyph of Howling Blast",	45806 },
    { "Glyph of Life Tap",	63941 },
    { "Glyph of Maul",		40897 },
    { "Glyph of Thorns",	57862 },
    { "Glyph of Totem of Wrath",63926 },
}, knownSpells[] = {
    { "Rising Sun Kick",	107248 },
    { "Tiger Palm",		100787 },
    { "Jab",			100780 },
    { "Tiger Power",		125359 },
    { "Tigereye Brew",		116740 },
    { "Fists of Fury",		117418 },
    { "Blackout Kick",		100784 },
    { "Combo Breaker",		115636 },
    { "Energizing Brew",	115288 },
    { "Flying Serpent Kick",	123586 },
    { "Soul Reaper",		130735 },
    { "Murder of Crows",	131894 },
    { "Dire Beast",		120679 },
    { "Glaive Toss",		117050 },
    { "Stampede",		121818 },
    { "Tricks of the Trade",	57934 },
    { "Blood Frenzy",		29836 },
    { "Tear Armor",		50498 },
    { "Fire Blast",		 2136 },
    { "Corrosive Spit",		35387 },
    { "Blessing of Kings",	20217 },
    { "Blood Boil",		48721 },
    { "Blade Barrier",		49182 },
    { "Dark Command",		56222 },
    { "Death Grip",		49576 },
    { "Blood Tap",		45529 },
    { "Freeze",			33395 },
    { "Archangel",		87151 },
    { "Evangelism",		81659 },
    { "Abominable Might",	55972 },
    { "Abomination's Might",	53137 },
    { "Adrenaline Rush",	13750 },
    { "Aimed Shot",		19434 },
    { "Ambush",			 8676 },
    { "Ancient Hysteria",	90355 },
    { "Alter Time",		108978 },
    { "Arcane Barrage",		44425 },
    { "Arcane Blast",		30451 },
    { "Arcane Brilliance",	 1459 },
    { "Arcane Missiles",	 5143 },
    { "Arcane Missiles2",	 79683},
    { "Arcane Power",		12042 },
    { "Arcane Charge",		114664 },
    { "Arcane Shot",		 3044 },
    { "Aspect of the Cheetah",	 5118 },
    { "Aspect of the Fox",	82661 },
    { "Aspect of the Hawk",	13165 },
    { "Aspect of the Iron Hawk",	109260 },
    { "Aspect of the Pack",	13159 },
    { "Aspect of the Wild",	20043 },
    { "Auto Shot",		   75 },
    { "Incanter's Ward",	1463 },
    { "Backstab",		   53 },
    { "Bane of Agony",		  980 },
    { "Bane of Doom",		  603 },
    { "Bane of Havoc",		80240 },
    { "Bane",			17788 },	// warlock talent
    { "Barkskin",		22812 },
    { "Battle Shout",		 6673 },
    { "Bear Form",		 5487 },
    { "Berserk",		   43 },
    { "Bestial Wrath",		19574 },
    { "Black Arrow",		 3674 },
    { "Blade Flurry",		13877 },
    { "Blessing of Might",	19740 },
    { "Blood Plague", 		59879 },
    { "Blood Presence",		48263 },
    { "Blood Shield",		77513 },
    { "Blood Strike", 		45902 },
    { "Blood in the Water",	80863 },
    { "Bloodlust",		 2825 },
    { "Bloodsurge",			46916 },
    { "Bloodthirst",		23880 },
    { "Bone Shield",		49222 },
    { "Brain Freeze",		44546 },
    { "Call Pet",		  883 },
    { "Cat Form",		  768 },
    { "Chaos Bolt",		50796 },
    { "Chimera Shot",		53209 },
    { "Clearcasting",		12536 },
    { "Cobra Shot",		77767 },
    { "Cold Blood",		14177 },
    { "Combustion",		11129 },
    { "Commanding Shout",	  469 },
    { "Concentration Aura",	19746 },
    { "Conflagrate",		17962 },
    { "Consecration",		26573 },
    { "Corruption",		  172 },
    { "Cremation",		85104 },	// warlock talent
    { "Critical Mass",		11095 },
    { "Crusader Aura",		32223 },
    { "Crusader Strike",	14517 },
    { "Curse of Exhaustion",	18223 },
    { "Curse of Tongues",	 1714 },
    { "Curse of Weakness",	  702 },
    { "Curse of the Elements",	 1490 },
    { "Cut to the Chase",	51664 },	// rogue talent
    { "Dalaran Brilliance",	61316 },
    { "Dancing Rune Weapon",	49028 },
    { "Dark Intent",		80398 },
    { "Dark Transformation",	93426 },
    { "Deadly Poison",		43581 },	// rogue debuff
    { "Death Coil",		47541 },
    { "Death Strike",		49998 },
    { "Death Wish",		12292 },
    { "Death and Decay",	43265 },
    { "Decimation",		63156 },
    { "Deep Freeze",		44572 },
    { "Demon Armor",		  687 },
    { "Demon Soul",		77801 },
    { "Demonic Empowerment",	47193 },
    { "Demoralizing Roar",	   99 },
    { "Demoralizing Shout",	 1160 },
    { "Devastate",		20243 },
    { "Devotion Aura",		  465 },
    { "Devouring Plague",	 2944 },
    { "Disengage",		  781 },
    { "Divine Plea",		54428 },
    { "Divine Storm",		53385 },
    { "Drain Life",	 	  689 },
    { "Drain Soul",	 	 1120 },
    { "Earth Shock",		 8042 },
    { "Eclipse (Lunar)",	48518 },
    { "Eclipse (Solar)",	48517 },
    { "Empower Rune Weapon",	47568 },
    { "Empowered Imp",		47283 },
    { "Empowered Shadow",	95799 },
    { "Energetic Recovery",	79150 },	// rogue talent
    { "Enrage",	5229 },
    { "Envenom",		32645 },
    { "Eviscerate", 		 2098 },
    { "Evocation",	12051 },
    { "Execute",	5308 },
    { "Exorcism",	879 },
    { "Explosive Shot",	15495 },
    { "Expose Armor",		 8647 },
    { "Faerie Fire (Feral)",	16857 },
    { "Faerie Fire",	770 },
    { "Faerie Swarm",	102354 },
    { "Omen of Clarity",	16864 },
    { "Ravage",		102545 },
    { "Incarnation",	106731 },
    { "Feed Pet",	1539 },
    { "Fel Armor",	28176 },
    { "Mirror Image",	55342 },
    { "Heating Up",	48107 },
    { "Inferno Blast",	108853 },
    { "Nether Tempest",	114954 },
    { "Feral Spirit",	555 },
    { "Ferocious Bite",	22568 },
    { "Fervor",			82726 },
    { "Festering Strike",	85948 },
    { "Fingers of Frost",	44545 },
    { "Fire Nova",		 1535 },
    { "Fire!",			82926 },
    { "Fireball",	133 },
    { "Flame Shock",		 8050 },
    { "Flametongue Weapon",	 8024 },
    { "Focus Fire",		82692 },
    { "Force of Nature",	6913 },
    { "Freezing Fog", 59052 },
    { "Frenzy Effect",		19615 },
    { "Frost Armor",		 7302 },
    { "Execution Sentence",	114157 },
    { "Frost Fever", 59921 },
    { "Frost Bomb",		112948 },
    { "Frost Presence", 	48266 },
    { "Frost Strike", 49143 },
    { "Frostbolt",	116 },
    { "Frostfire Bolt",	44614 },
    { "Frostfire Orb",		84726 },
    { "Garrote",		  703 },
    { "Ghoul Summons",	45850 },
    { "Growl",	1853 },
    { "Hammer of Wrath",	24275 },
    { "Hand of Gul'dan",	71521 },
    { "Haunt",	48181 },
    { "Health Funnel",	755 },
    { "Heart Strike",		55050 },
    { "Hemorrhage",		16511 },
    { "Heroic Strike",	78 },
    { "Heroism",	23682 },
    { "Holy Light",	635 },
    { "Holy Wrath",	2812 },
    { "Horn of Winter", 57330 },
    { "Hot Streak",	44445 },
    { "Howling Blast", 49184 },
    { "Hunter's Mark",	1130 },
    { "Ice Armor",	1214 },
    { "Incanter's Ward",	1463 },
    { "Ice Lance",		30455 },
    { "Icy Touch", 45477 },
    { "Icy Veins",	12472 },
    { "Immolate",	348 },
    { "Immolation Aura",	50589 },
    { "Improved Soul Fire",	85383 },	// buff, not warlock talent
    { "Improved Steady Shot",	53221 },	// hunter talent
    { "Incinerate",	19397 },
    { "Inner Fire",		  588 },
    { "Inquisition",		84963 },
    { "Insect Swarm",	5570 },
    { "Judgement",		20271 },
    { "Kill Command",	34026 },
    { "Kill Shot",	53351 },
    { "Killing Machine", 51124 },
    { "Killing Spree", 		51690 },
    { "Lacerate",	5422 },
    { "Lava Burst",	51505 },
    { "Lava Lash",	60103 },
    { "Life Tap",	1454 },
    { "Lightning Bolt",	403 },
    { "Lightning Shield",	324 },
    { "Living Bomb",	44457 },
    { "Maelstrom Weapon",	51528 },
    { "Mage Armor",	6117 },
    { "Flametongue Totem",	 8227 },
    { "Magma Totem",	8187 },
    { "Fire Elemental Totem",	 2894 },
    { "Tendon Rip",	50271 },
    { "Mangle",		33878 },
    { "Mark of the Wild",	1126 },
    { "Marked for Death",	53241 },
    { "Mastery",		86473 },	
    { "Maul",	6807 },
    { "Mend Pet",	136 },
    { "Metamorphosis",	36298 },
    { "Might of the Frozen Wastes",	81330 },
    { "Mind Blast",	8092 },
    { "Mind Flay",	15407 },
    { "Mind Spike",   73510 },
    { "Molten Armor",	30482 },
    { "Molten Core",	47245 },
    { "Moonfire",	563 },
    { "Moonkin Form",	24858 },
    { "Multi-Shot",	2643 },
    { "Multi-Shot",	2643 },                    
    { "Murderous Intent",	14158 },	// rogue talent
    { "Mutilate",		 1329 },
    { "Gore",		35290 },
    { "Nature's Grace",	16886 },
    { "Nether Tempest",	114923 },
    { "Nightfall",	18094 },
    { "Obliterate", 49020 },
    { "Outbreak",		77575 },
    { "Overkill",		58426 },
    { "Overpower",	7384 },
    { "Pestilence",		50842 },
    { "Pillar of Frost",	51271 },	
    { "Plague Strike", 45462 },
    { "Poisons",		 2842 },
    { "Power Word: Fortitude",	21562 },
    { "Power Word: Shield",	17 },
    { "Premeditation",		14183 },
    { "Preparation",		14185 },
    { "Presence of Mind",	12043 },
    { "Pulverize",		80313 },
    { "Pyroblast",	11366 },
    { "Raise Dead",	46584 },
    { "Rake",	1822 },
    { "Rapid Fire",	3045 },
    { "Raptor Strike",	2973 },
    { "Readiness",	23989 },
    { "Recklessness",		 1719 },
    { "Recuperate",		73651 },		
    { "Rend",	772 },
    { "Resistance Aura",	19891 },
    { "Retribution Aura",	7294 },
    { "Revealing Strike", 	84617 },
    { "Revenge",	6572 },
    { "Revive Pet",	982 },
    { "Rip",	1079 },
    { "Roar of Courage",	93435 },
    { "Rune Strike",		56815 },
    { "Rune of Power",	116011},
    { "Rune of Powerb",	116014},
    { "Rupture",		 1943 },
    { "Savage Roar",	52610 },
    { "Scorch",	2948 },
    { "Scourge Strike",		55090 },
    { "Seal of Insight",	20165 },
    { "Seal of Justice",	20164 },
    { "Seal of Righteousness",	20154 },
    { "Seal of Truth",	31801 },
    { "Searing Totem",	3599 },
    { "Seed of Corruption",	27243 },
    { "Serpent Sting",	1978 },
    { "Shadow Bolt",	686 },
    { "Shadow Dance",		51713 },
    { "Shadow Embrace",		32385 },	// warlock talent
    { "Shadow Orb",		77487 },	
    { "Shadow Trance",	17941 },
    { "Shadow Word: Death",	32379 },
    { "Shadow Word: Pain",	589 },
    { "Shadow and Flame",	17793 },
    { "Shadowburn",	17877 },
    { "Shadowfiend",		34433 },
    { "Shadowflame",		47897 },
    { "Shadowform",	15473 },
    { "Shadowstep", 36554 },
    { "Shamanistic Rage",	30823 },
    { "Shield Slam",	8242 },
    { "Shoot",			 5019 },
    { "Shred",	3252 },
    { "Silencing Shot",	34490 },
    { "Sinister Strike",	 1752 },
    { "Slam",	1464 },
    { "Slice and Dice",		 5171 },
    { "Solar Beam",		78675 },
    { "Soul Fire",	6353 },
    { "Soul Harvest",		79268 },
    { "Soulburn",		74434 },
    { "Starfall",		50288 },
    { "Starfire",	2912 },
    { "Starsurge",		78674 },
    { "Steady Shot",		56641 },
    { "Stealth",		 1784 },
    { "Stormstrike",	17364 },
    { "Strength of Earth Totem",	8075 },
    { "Sudden Doom",		49018 },
    { "Summon Doomguard",	18540 },
    { "Summon Felguard",	30146 },
    { "Summon Felhunter",	691 },
    { "Summon Gargoyle",	49206 },
    { "Surge of Darkness",   87160},
    { "Summon Imp",	688 },
    { "Summon Succubus",	712 },
    { "Summon Water Elemental",	17162 },
    { "Sunder Armor",	7386 },
    { "Sunfire",		93402 },
    { "Swipe",	779 },
    { "Taunt",	355 },
    { "Templar's Verdict",	85256 },
    { "The Art of War",	53486 },
    { "Thorns",	467 },
    { "Thrash",			77758 },
    { "Thunder Clap",	6343 },
    { "Thunderclap",	8078 },
    { "Tiger's Fury",	5217 },
    { "Time Warp",		80353 },
    { "Trauma",			46856 },
    { "Trueshot Aura",	19506 },
    { "Typhoon",		50516 },
    { "Unholy Presence", 	48265 },
    { "Unleash Elements", 73680 },
    { "Unleash Flame",	73683 },
    { "Unleashed Rage",	30802 },
    { "Unstable Affliction",	30108 },
    { "Vampiric Embrace",	15286 },
    { "Vampiric Touch",	34914 },
    { "Vanish",			 1856 },
    { "Vendetta",		79140 },
    { "Victory Rush",	34428 },
    { "Water Shield",		52127 },
    { "Weakened Armor",		113746 },
    { "Whirlwind",	1680 },
    { "Windfury Weapon",	8232 },
    { "Wing Clip",	2974 },
    { "Winter's Chill",		11180 },
    { "Wrath",	5176 },
    { "Wyvern Sting",		19386 },
    { "Zealotry",		85696 },
    { "Cascade",		121135 },
    { "Divine Star",		110744 },
    { "Halo",		120517 },
    { "Mind Sear",		48045 },
    { "Mindbender",		123040},
};

#define NELEM(x)	(sizeof(x) / sizeof(*(x)))

static int
compar (const void *p1, const void *p2)
{
    const struct spellInfo *sp1 = p1, *sp2 = p2;

    return strcasecmp(sp1->name, sp2->name);
}

int
isSpell (char *string)
{
    char stripped[1024];
    struct spellInfo target;

    // strip off leading and trailing quotes
    strcpy(stripped, &string[1]);
    stripped[strlen(stripped) - 1] = '\0';

    target.name = stripped;
    target.number = 0;

    // check spells
    if (bsearch(&target, knownSpells, NELEM(knownSpells), 
                sizeof(*knownSpells), compar) != NULL)
	return 1;

    // check items
    if (bsearch(&target, knownItems, NELEM(knownItems), 
                sizeof(*knownItems), compar) != NULL)
	return 1;

    // check things
    if (bsearch(&target, knownThings, NELEM(knownThings), 
                sizeof(*knownThings), compar) != NULL)
	return 1;

    return 0;
}

void
verifyStrings (void)
{
    qsort(knownSpells, NELEM(knownSpells), sizeof(*knownSpells), compar);
    qsort(knownItems, NELEM(knownItems), sizeof(*knownItems), compar);
    qsort(knownThings, NELEM(knownThings), sizeof(*knownThings), compar);
}

void
genLocale (void)
{
    int i;

    printf("-- Warning: this file is autogenerated.  All changes made here\n");
    printf("-- will be lost\n\n");
    printf("local spellIndex = 0\n");
    printf("local itemIndex = 0\n");
    printf("local English2SpellIndex = {};\n");
    printf("local English2ItemIndex = {};\n");
    printf("local knownSpellIndex = {};\n\n");
    printf("local knownItemIndex = {};\n\n");

    for (i = 0 ; i < NELEM(knownSpells) ; i++) {
	if (knownSpells[i].number != -1)
	    printf("English2SpellIndex[\"%s\"] = %d;\n", knownSpells[i].name, knownSpells[i].number);
    }

    for (i = 0 ; i < NELEM(knownItems) ; i++) {
	if (knownItems[i].number != -1)
	    printf("English2ItemIndex[\"%s\"] = %d;\n", knownItems[i].name, knownItems[i].number);
    }

    for (i = 0 ; i < NELEM(knownThings) ; i++) {
	if (knownThings[i].number != -1)
	    printf("English2ThingIndex[\"%s\"] = %d;\n", knownThings[i].name, knownThings[i].number);
    }

    printf("\n");
    for (i = 0 ; i < NELEM(knownSpells) ; i++) {
	printf("knownSpellIndex[spellIndex] = \"%s\";\tspellIndex = spellIndex + 1;\n",
	       knownSpells[i].name);
    }
    for (i = 0 ; i < NELEM(knownItems) ; i++) {
	printf("knownItemIndex[itemIndex] = \"%s\";\titemIndex = itemIndex + 1;\n",
	       knownItems[i].name);
    }

    printf("\n");
    printf("\n");
    printf("----------------------------------------------\n");
    printf("--\n");
    printf("--	Learn all the local names\n");
    printf("--\n");
    printf("\n");
    printf("for cur = 0, (spellIndex - 1) do\n");
    printf("    local curSpid, curSpell;\n");
    printf("    curSpell = knownSpellIndex[cur];\n");
    printf("    curSpid = English2SpellIndex[curSpell];\n");
    printf("\n");
    printf("    local name = GetSpellInfo(curSpid);\n");
    printf("    if name == nil then\n");
    printf("        print(\"Destruction for Dummies: cannot find spell info on \", curSpell);\n");
    printf("    else\n");
    printf("        DestroDummies.string[curSpell] = name;\n");
    printf("    end\n");
    printf("end\n");
    printf("\n");
    printf("for cur = 0, (itemIndex - 1) do\n");
    printf("    local curItemId, curItem;\n");
    printf("    curItem = knownItemIndex[cur];\n");
    printf("    curItemId = English2ItemIndex[curItem];\n");
    printf("\n");
    printf("    local name = GetItemInfo(curItemId);\n");
    printf("    if name == nil then\n");
    printf("        print(\"Destruction for Dummies: cannot find item info on \", curItem);\n");
    printf("    else\n");
    printf("        DestroDummies.string[curItem] = name;\n");
    printf("    end\n");
    printf("end\n");

    // learn about the different itemType and itemSubtype strings, as returned
    // by GetItemInfo()
    //
    // we do that by knowing one of each type of thing, and looking it up
    {
	struct {
	    char *english;
	    int exampleId;
	} stuff[] = {
	    { "Daggers",  		25101 },
	    { "Fist Weapons", 		17738 },
	    { "One-Handed Axes",  	  811 },
	    { "Two-Handed Axes",  	  870 },
	    { "One-Handed Swords", 	 8190 },
	    { "Two-Handed Swords",	 2084 },
	    { "One-Handed Maces",	36694 },
	    { "Two-Handed Maces", 	25135 },
	    { "Staves", 		50040 },
	    { "Guns", 			25639 },
	    { "Bows", 			28772 },
	    { "Crossbows", 		28504 },
	    { "Thrown", 		27916 },
	    { "Wands", 			28673 },
	};
	int i;

	printf("\n\n");
	printf("-- Get the localized names for weapons\n");
	printf("local itemType, itemSubtype;\n");
	printf("_,_,_,_,_, itemType = GetItemInfo(%d);\n",
	       stuff[0].exampleId);
	printf("DestroDummies.string[\"Weapon\"] = itemType;\n");
	for (i = 0 ; i < NELEM(stuff) ; i++) {
	    printf("_,_,_,_,_,_, itemSubtype = GetItemInfo(%d);\n",
		   stuff[i].exampleId);
	    printf("DestroDummies.string[\"%s\"] = itemSubtype;\n",
		   stuff[i].english);
	}
    }   
	    
    printf("\n");
    printf("\n");
    printf("-- Translate between the English strings we use internally and the localized \n");
    printf("-- strings.  Return the English for strings for which we do not have\n");
    printf("-- translations.\n");
    printf("function DestroDummies:xlateString (string)\n");
    printf("    if DestroDummies.string[string] == nil then\n");
    printf("        -- We are missing a translation.  Just return the English\n");
    printf("        return string;\n");
    printf("    end\n");
    printf("\n");
    printf("    return DestroDummies.string[string];\n");
    printf("end\n");

    printf("\n\n");
    printf("-- A debugging routine to find a spell number, given the name\n");
    printf("function DestroDummies:findSpellNo (string)\n");
    printf("    local cur, max, spell;\n");
    printf("\n");
    printf("    max = 200000		-- the highest number to try\n");
    printf("    for cur = 0, max do\n");
    printf("        spell = GetSpellInfo(cur);\n");
    printf("	if spell == string then\n");
    printf("	    print(\"Spell \", spell, \" is number \", cur);\n");
    printf("	    return;\n");
    printf("    	end\n");
    printf("    end\n");
    printf("    print(\"Spell \", string, \" is unknown\");\n");
    printf("end\n");

    printf("\n\n");
    printf("-- A debugging routine to verify that all the spell numbers are correct\n");
    printf("-- This is never called--it is provided as a test mechanism for the author\n");
    printf("function DestroDummies:verifyCodes ()\n");
    printf("    local cur, curSpid, curSpell;\n");
    printf("    for cur = 0, (spellIndex - 1) do\n");
    printf("        curSpell = knownSpellIndex[cur];\n");
    printf("        curSpid = English2SpellIndex[curSpell];\n");
    printf("\n");
    printf("        local name = GetSpellInfo(curSpid);\n");
    printf("        if name ~= curSpell then\n");
    printf("            print(\"Destruction for Dummies: incorrect spell number \", curSpid, \": my name: \", curSpell, \", WoW Name: \", name);\n");
    printf("        end\n");
    printf("    end\n");
    printf("end\n");


}
