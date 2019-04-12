#pragma once

#include <other/BaseMod.h>
#include <utility/Connection.h>
#include <utility/Dispatcher.h>
#include <utility/Logger.h>
#include <other/CommandParser.h>
#include <other/CommandSystem.h>

class FlorensFun : SML::BaseMod {
public:
	SML::Globals* globalsReference;

	const char* Name() { 
		return "Florens' Fun"; 
	};
	const char* Version() { 
		return "0.1"; 
	};
	const char* Description() { 
		return "Just a bunch of fun stuff."; 
	};
	const char* Authors() { 
		return "Florens"; 
	};
	virtual const std::vector<const char*> Dependencies() {
		return std::vector<const char*>{
			// list here
		};
	}

	void PreSetup(SML::Globals* globals) {
		SML::mod_info(Name(), "Setting up ", Name());
		globalsReference = globals;

		Setup();
	}

	void Cleanup();

protected:
	void Setup();
};

GLOBAL FlorensFun* CreateMod() {
	return new FlorensFun;
}