# add -d for debugging
DDDL_FLAGS = 	

# if you have your AddOn directory mounted where your Linux system can
# directly copy to it, these defines could be useful
SUDO		= /usr/bin/sudo
CP		= /bin/cp		# maybe scp would be better for you?
ADDON_DIR	= ~/WoW/AddOns/DestructionForDummies



# Nothing below here should need modification, unless you are adding files

ADDON_NAME =	DestructionForDummies

.SUFFIXES: .lua .dddl .y .c .l .o

.dddl.lua: dddl/dddl
	dddl/dddl $(DDDL_FLAGS) -i $<  > $@.tmp
	mv $@.tmp $@

TARGETS =  	LocaleLocal.lua			\
		DeathKnightBlood.lua		\
		DeathKnightFrost.lua		\
		DeathKnightUnholy.lua		\
		DruidBalance.lua		\
		DruidFeral.lua 			\
		HunterBeastmaster.lua		\
		HunterMarksman.lua		\
		HunterSurvival.lua		\
		MageArcane.lua			\
		MageFire.lua			\
		MageFrost.lua			\
		MonkWindwalker.lua		\
		PaladinRetribution.lua		\
		PriestShadow.lua		\
		RogueAssassination.lua		\
		RogueCombat.lua			\
		RogueSubtlety.lua		\
		ShamanElemental.lua		\
		ShamanEnhancement.lua		\
		WarlockAffliction.lua		\
		WarlockDemonology.lua		\
		WarlockDestruction.lua		\
		WarriorFury.lua			\
		WarriorProtection.lua

FILES 	=  	DestructionForDummies.toc 	\
	   	DestructionForDummies.lua 	\
		Classes.lua			\
		LocaleEnUS.lua			\
		LocaleDeDE.lua			\
		LocaleFrFR.lua			\
		Utilities.lua			\
	   	Variables.lua 			\
	   	$(TARGETS)

VPATH = src

all: do_dddl $(TARGETS)

do_dddl:
	(cd dddl; make dddl)

LocaleLocal.lua: dddl/dddl
	dddl/dddl -l > tmpLocale
	mv tmpLocale LocaleLocal.lua

release: all
	mkdir $(ADDON_NAME)
	cp -r dddl src Makefile ReadMe $(ADDON_NAME)
	(cd $(ADDON_NAME); make clean; rm -rf CVS */CVS)
	cp $(FILES) $(ADDON_NAME)
	sh -c "zip -r 'DestructionForDummies'`grep Version DestructionForDummies.toc | sed -e 's/^## Version: *//' -e 's/\./_/g'`'.zip' $(ADDON_NAME)/*"
	rm -rf $(ADDON_NAME)

clean:
	rm -f *~ */*~ \#* $(TARGETS) tmpLocale
	rm -rf $(ADDON_NAME)
	cd dddl ; make clean

# this rule exists just for convenience--it assumes that you have your 
# AddOns directory mounted so it can be viewed by your Linux system.
install: all
	$(SUDO) $(CP) $(FILES) $(ADDON_DIR)

*.lua: dddl/dddl

# include dependencies
DeathKnightBlood.lua: src/mutexes.h
DeathKnightFrost.lua: src/mutexes.h
DeathKnightUnholy.lua: src/mutexes.h
DruidFeral.lua: src/mutexes.h
