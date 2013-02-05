class: shaman
build: enhancement

mutex shield (clip = 60, 5) {
    "Lightning Shield",
    "Water Shield",
};

InCombat {
    clip {
	"Windfury Weapon": 5;
    };

    alternate (1) { "Heroism": 		Castable(); };
    alternate (3) { "Feral Spirit": 	Castable() && !Boss(); };

    var mTime, mCount;
    mTime, mCount = BuffInfo("Maelstrom Weapon");

    "Windfury Weapon":		!WeaponEnhancement();

    "Feral Spirit":		Boss();
    "Lightning Bolt":		mCount == 5;
    "Earth Shock":		Debuff("Storm Strike");
    "Stormstrike":		Castable();
    "Earth Shock";
    "Magma Totem":		!TotemFire();
    "Lightning Shield":		!MxBuff(shield);
    "Lava Lash";
    "Lightning Bolt":		mCount > 3;
}

OutOfCombat {
    clip {
	"Windfury Weapon": 5 * 60;
    };

    if (!Mounted()) {
	"Windfury Weapon":		!WeaponEnhancement();
	"Lightning Shield":		!MxBuff(shield);
    }
}
