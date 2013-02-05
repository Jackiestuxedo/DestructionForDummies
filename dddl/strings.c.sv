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
}, knownSpells[] = {
    { "Glyph of Thorns",	57862 },
    { "Glyph of Disease",	63959 },
    { "Glyph of Life Tap",	63941 },
    { "Glyph of Scorch",	56595 },
    { "Glyph of Totem of Wrath",63926 },

    { "Blood in the Water",	80863 },
    { "Fire!",			82926 },
    { "Soul Harvest",		79268 },
    { "Bane of Agony",		  980 },
    { "Bane of Doom",		  603 },
    { "Curse of Agony",		  980 },
    { "Curse of Doom",		  603 },
    { "Aspect of the Fox",	82661 },
    { "Cobra Shot",		77767 },
    { "Barkskin",		22812 },
    { "Frenzy Effect",		19615 },
    { "Fervor",			82726 },
    { "Focus Fire",		82692 },
    { "Disengage",		  781 },
    { "Blessing of Might",	19740 },
    { "Greater Blessing of Might", 25782 },
    { "Slam!",			46916 },
    { "Recklessness",		 1719 },
    { "Totem of Wrath",		30706 },
    { "Flametongue Weapon",	 8024 },
    { "Fire Nova",		 1535 },
    { "Flame Shock",		 8050 },
    { "Shamanistic Rage",	30823 },
    { "Misery",			33191 },
    { "Shadow Weaving",		15332 },
    { "Vampiric Embrace",	15286 },
    { "Divine Spirit",		14752 },
    { "Inner Fire",		  588 },
    { "Arcane Barrage",		44425 },
    { "Presence of Mind",	12043 },
    { "Deep Freeze",		44572 },
    { "Fireball!",		57761 },
    { "Abominable Might",	55972 },
    { "Abomination's Might",	53137 },
    { "Rune Strike",		56815 },
    { "Bone Shield",		49222 },
    { "Pestilence",		50842 },
    { "Death Strike",		49998 },
    { "Heart Strike",		55050 },
    { "Scourge Strike",		55090 },
    { "Death Coil",		47541 },
    { "Summon Gargoyle",	49206 },
    { "Scorpid Sting",		 3043 },
    { "Viper Sting",		 3034 },
    { "Wyvern Sting",		19386 },
    { "Aimed Shot",		19434 },
    { "Arcane Blast",		10833 },
    { "Arcane Brilliance",	23028 },
    { "Arcane Intellect",	 1459 },
    { "Arcane Missiles",	 5143 },
    { "Arcane Shot",		 3044 },
    { "Aspect of the Beast",	13161 },
    { "Aspect of the Cheetah",	 5118 },
    { "Aspect of the Dragonhawk",	61846 },
    { "Aspect of the Hawk",	13165 },
    { "Aspect of the Monkey",	13163 },
    { "Aspect of the Pack",	13159 },
    { "Aspect of the Viper",	34074 },
    { "Aspect of the Wild",	20043 },
    { "Auto Shot",	75 },
    { "Battle Shout",	2048 },
    { "Bear Form",	5487 },
    { "Berserk",	43 },
    { "Bestial Wrath",	19574 },
    { "Black Arrow",	3674 },
    { "Blood Plague", 59879 },
    { "Blood Presence",	48266 },
    { "Blood Strike", 45902 },
    { "Bloodrage",	2687 },
    { "Bloodthirst",	23880 },
    { "Call Pet",	883 },
    { "Cat Form",	768 },
    { "Chaos Bolt",	50796 },
    { "Chimera Shot",	53209 },
    { "Clearcasting",	12536 },
    { "Combustion",	11129 },
    { "Commanding Shout",	469 },
    { "Concentration Aura",	19746 },
    { "Conflagrate",	17962 },
    { "Consecration",	20116 },
    { "Corruption",	172 },
    { "Create Firestone",	6366 },
    { "Create Spellstone",	2362 },
    { "Crusader Aura",	32223 },
    { "Crusader Strike",	14517 },
    { "Curse of Exhaustion",	18223 },
    { "Curse of Tongues",	1714 },
    { "Curse of Weakness",	702 },
    { "Curse of the Elements",	1490 },
    { "Dalaran Brilliance",	61316 },
    { "Dalaran Intellect",	61024 },
    { "Death Wish",	12292 },
    { "Decimation",	63156 },
    { "Demon Armor",	706 },
    { "Demonic Empowerment",	47193 },
    { "Demoralizing Roar",	99 },
    { "Demoralizing Shout",	1160 },
    { "Devastate",	20243 },
    { "Devotion Aura",	465 },
    { "Devouring Plague",	2944 },
    { "Dire Bear Form",	9634 },
    { "Divine Storm",	53385 },
    { "Drain Soul",	1120 },
    { "Earth Shock",	8042 },
    { "Eclipse (Lunar)",	48518 },
    { "Eclipse (Solar)",	48517 },
    { "Eclipse",	48516 },
    { "Enrage",	5229 },
    { "Evocation",	12051 },
    { "Execute",	5308 },
    { "Exorcism",	879 },
    { "Explosive Shot",	15495 },
    { "Faerie Fire (Feral)",	16857 },
    { "Faerie Fire (Feral)()",	16857 },
    { "Faerie Fire",	770 },
    { "Feed Pet",	1539 },
    { "Fel Armor",	28176 },
    { "Feral Spirit",	555 },
    { "Ferocious Bite",	22568 },
    { "Fire Resistance Aura",	19891 },
    { "Fireball",	133 },
    { "Force of Nature",	6913 },
    { "Freezing Fog", 59052 },
    { "Frost Armor",	168 },
    { "Frost Fever", 59921 },
    { "Frost Resistance Aura",	19888 },
    { "Frost Strike", 49143 },
    { "Frostbolt",	116 },
    { "Frostfire Bolt",	44614 },
    { "Ghoul Summons",	45850 },
    { "Gift of the Wild",	21849 },
    { "Growl",	1853 },
    { "Hammer of Wrath",	24239 },
    { "Haunt",	48181 },
    { "Health Funnel",	755 },
    { "Heroic Strike",	78 },
    { "Heroism",	23682 },
    { "Holy Light",	635 },
    { "Holy Wrath",	2812 },
    { "Horn of Winter", 57330 },
    { "Hot Streak",	44445 },
    { "Howling Blast", 49184 },
    { "Hunter's Mark",	1130 },
    { "Ice Armor",	1214 },
    { "Icy Touch", 45477 },
    { "Icy Veins",	12472 },
    { "Immolate",	348 },
    { "Improved Scorch",	11095 },
    { "Incinerate",	19397 },
    { "Insect Swarm",	5570 },
    { "Judgement of Light",	20271 },
    { "Judgement of Wisdom",	53408 },
    { "Kill Command",	34026 },
    { "Kill Shot",	53351 },
    { "Killing Machine", 51123 },
    { "Lacerate",	5422 },
    { "Lava Burst",	51505 },
    { "Lava Lash",	60103 },
    { "Life Tap",	1454 },
    { "Lightning Bolt",	403 },
    { "Lightning Shield",	324 },
    { "Living Bomb",	20475 },
    { "Maelstrom Weapon",	51528 },
    { "Mage Armor",	6117 },
    { "Magma Totem",	8187 },
    { "Mangle (Bear)",	33878 },
    { "Mangle (Bear)()",	33878 },
    { "Mangle (Cat)",	33876 },
    { "Mangle (Cat)()",	33876 },
    { "Mark of the Wild",	1126 },
    { "Maul",	6807 },
    { "Mend Pet",	136 },
    { "Metamorphosis",	36298 },
    { "Mind Blast",	8092 },
    { "Mind Flay",	15407 },
    { "Missile Barrage",	44401 },
    { "Mocking Blow",	694 },
    { "Molten Armor",	30482 },
    { "Molten Core",	47245 },
    { "Mongoose Bite",	1495 },
    { "Moonfire",	563 },
    { "Moonkin Form",	24858 },
    { "Multi-Shot",	2643 },
    { "Multi-Shot",	2643 },
    { "Nature's Grace",	16886 },
    { "Nightfall",	18094 },
    { "Obliterate", 66198 },
    { "Overpower",	7384 },
    { "Plague Strike", 45462 },
    { "Power Word: Shield",	17 },
    { "Power Word: Fortitude",	1243 },
    { "Prayer of Fortitude",	21562 },
    { "Pyroblast",	11366 },
    { "Raise Dead",	46584 },
    { "Rake",	1822 },
    { "Rapid Fire",	3045 },
    { "Raptor Strike",	2973 },
    { "Readiness",	23989 },
    { "Rend",	772 },
    { "Retribution Aura",	7294 },
    { "Revenge",	6572 },
    { "Revive Pet",	982 },
    { "Rip",	1079 },
    { "Savage Roar",	52610 },
    { "Scorch",	1811 },
    { "Seal of Command",	20375 },
    { "Seal of Corruption",	53736 },
    { "Seal of Justice",	20164 },
    { "Seal of Light",	20165 },
    { "Seal of Righteousness",	20154 },
    { "Seal of Vengeance",	31801 },
    { "Seal of Wisdom",	20166 },
    { "Seed of Corruption",	27243 },
    { "Serpent Sting",	1978 },
    { "Shadow Bolt",	686 },
    { "Shadow Resistance Aura",	19876 },
    { "Shadow Trance",	17941 },
    { "Shadow Word: Pain",	589 },
    { "Shadowburn",	17877 },
    { "Shadowform",	15473 },
    { "Shield Slam",	8242 },
    { "Shred",	3252 },
    { "Silencing Shot",	34490 },
    { "Slam",	1464 },
    { "Soul Fire",	6353 },
    { "Starfire",	2912 },
    { "Steady Shot",	34120 },
    { "Storm Strike",	17364 },
    { "Stormstrike",	17364 },
    { "Summon Felguard",	30146 },
    { "Summon Felhunter",	691 },
    { "Summon Imp",	688 },
    { "Summon Succubus",	712 },
    { "Summon Water Elemental",	17162 },
    { "Sunder Armor",	7386 },
    { "Swipe (Bear)()",	779 },
    { "Taunt",	355 },
    { "The Art of War",	53486 },
    { "Thorns",	467 },
    { "Thunder Clap",	6343 },
    { "Thunderclap",	8078 },
    { "Tiger's Fury",	5217 },
    { "Trauma",	46854 },
    { "Trueshot Aura",	19506 },
    { "Unleashed Rage",	30802 },
    { "Unstable Affliction",	30108 },
    { "Vampiric Touch",	34914 },
    { "Victory Rush",	34428 },
    { "Water Shield",	23575 },
    { "Whirlwind",	1680 },
    { "Windfury Weapon",	8232 },
    { "Wing Clip",	2974 },
    { "Wrath",	5176 },
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
}
