// Hey emacs!  This is a -*- c -*- file

/*
 *  This file contains all the mutexes that hold shared (non-stackable)
 *  buffs and debuffs.  It is intended to be included by other files.
 *
 *  Mutexes for class-specific things (like all the different Hunter aspects)
 *  probably shouldn't go in here, as every file makes its own copy of these
 *  mutexes instead of relying on a common one.  Actually, I should fix that--
 *  it should be easy enough to assign a variable with the mutex's name
 *  and just dereference that.
 */

mutex MxBuffAgiStr {
    "Horn of Winter",
    "Strength of Earth Totem",
    "Battle Shout",
    "Roar of Courage",
};

mutex MxBuffMark (clip = 5) {
    "Mark of the Wild",
    "Blessing of Kings",
};

mutex MxDebuffBleed (clip = 1.5) {
    "Mangle",
    "Trauma",
};

mutex MxDebuffArmor {
    "Faerie Fire",
    "Faerie Swarm",
    "Tear Armor",
    "Corrosive Spit",
    "Expose Armor",
    "Sunder Armor",
};


