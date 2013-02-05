/* definitions to speed the processing of built-in functions */

enum built_in {
    BI_UNUSED = 0,

    BI_TOTEMTYPE,
    BI_TAPPED,
    BI_ONTANK,
    BI_UPTIME,
    BI_RAWPLAYERHEALTH,
    BI_RAWPLAYERMAXHEALTH,
    BI_FLOOR,
    BI_HAVE_RUNES,
    BI_COOLDOWN,
    BI_IN_RAID,
    BI_IN_PARTY,
    BI_HOLY_POWER,
    BI_LUNAR_POWER,
    BI_DISTANCE,
    BI_RUNE_COUNT,
    BI_DUAL_WIELDING,
    BI_TALENT,
    BI_SOULSHARDCOUNT,
    BI_BOSS,
    BI_BUFF,
    BI_BUFF_BY_ME,
    BI_BUFF_INFO,
    BI_DEBUFF_BY_ME_INFO,
    BI_PETBUFF_INFO,
    BI_BUFF_TIME,
    BI_CASTABLE,
    BI_COMBOPOINTS,
    BI_CREATURETYPE,
    BI_DEBUFF_BY_ANY,
    BI_DEBUFF_BY_ME,
    BI_DEBUFF_INFO,
    BI_DEBUFF_TIME,
    BI_PLAYER_DEBUFF,
    BI_PLAYER_DEBUFF_INFO,
    BI_REQUIREDPOWER,
    BI_RAWPOWER,
    BI_FERRALMODE,
    BI_FLAGGEDPVP,
    BI_FLIGHTTIME,
    BI_GCD,
    BI_HASGLYPH,
    BI_HASPET,
    BI_PETEXISTS,
    BI_INRANGE,
    BI_KNOWSSPELL,
    BI_LASTCAST,
    BI_MOUNTED,
    BI_MXBUFF,
    BI_MXBUFF_BY_ME,
    BI_MXDEBUFF,
    BI_MXDEBUFF_BY_ME,
    BI_MXNEED,
    BI_PETBUFF,
    BI_PETHAPPINESS,
    BI_PETHEALTH,
    BI_PLAYERHEALTH,
    BI_POWER,
    BI_RANGED,
    BI_SOLO,
    BI_STANCE,
    BI_TARGETHEALTH,
    BI_THREAT,
    BI_TIMETODIE,
    BI_TOTEM,
    BI_WEAPONENHANCE,
};
    
enum res {
    RES_NONE,
    RES_ENERGY,
    RES_MANA,
    RES_RAGE,
};

#define YYSTYPE	 union my_yystype

union my_yystype {
    double fl;
    char *str;
    int intg;
    enum built_in bi;
    enum res res;
};

extern int lineno;
extern char *filename;

char *astrdup(char *); 
void *amalloc(int);
void lex_init(FILE *);
void lex_register_variable(char *, int);
void lex_register_mutex(char *, int);
