#include <stdafx.h>
#include <utility/Configuration.h>
#include <other/CommandSystem.h>
#include <events/PlayerEvents.h>
#include <other/CommandSystem.h>
#include <other/CommandParser.h>
#include "FlorensFun.h"
#include "ModReturns.h"
#include "Internals.h"

using event = SML::HookLoader::Event;

// if you want to access the Global values
FlorensFun* mod;
SML::CommandSystem commandSystem;
SML::Configuration config("FlorensFun");

float gravity = -980;
float gravityStep = 25;
char gravityUpKey;
char gravityDownKey;


void GetMessage(SML::ModReturns* returns, void* player, std::string message) {
	SML::mod_info(mod->Name(), "Message recieved: ", message);

	SML::CommandParser::CommandData data = SML::CommandParser::Parse(message);
	SML::Command command = commandSystem.get_command(data.Command);
	if (!command.Empty) {
		SML::mod_info(mod->Name(), "Invoking Command: ", data.Command);
		command.Invoke(player, data);
		// cancel the original function
		returns->UseOriginalFunction = false;
	}
	else {
		SML::mod_info(mod->Name(), "Invalid Command: ", data.Command);
	}
}

void GetInput(SML::ModReturns* returns, void* input, void* key, void* event, float amountDepressed, bool gamepad) {
	if (GetAsyncKeyState(gravityUpKey)) {
		gravity -= gravityStep;
		SML::mod_info(mod->Name(), "Gravity up: ", gravity);
	} else if (GetAsyncKeyState(gravityDownKey)) {
		gravity += gravityStep;
		SML::mod_info(mod->Name(), "Gravity down: ", gravity);
	}
}

void GravityMod(SML::ModReturns* returns, void* worldSettings) {
	returns->ReturnValue = &gravity;
	returns->UseOriginalFunction = false;
}

/// Commands

void SetGravity(void* player, SML::CommandParser::CommandData data) {
	if (data.Args.capacity() < 1) {
		return;
	}

	gravity = data.get_float(0);

	std::string str("Set the gravity to " + std::to_string(gravity));
	auto pointer = mod->globalsReference->functions[event::AFGPlayerControllerEnterChatMessageGlobal];
	((void(__stdcall*)(void*, std::string))pointer)(player, str);
}

void GetGravity(void* player, SML::CommandParser::CommandData data) {
	std::string str("The current gravity is " + std::to_string(gravity));
	auto pointer = mod->globalsReference->functions[event::AFGPlayerControllerEnterChatMessageGlobal];
	((void(__stdcall*)(void*, std::string))pointer)(player, str);
}

void FlorensFun::Setup() {
	mod = this;

	if (!config.exists()) {
		config.set("DefaultGravity", gravity);
		config.set("GravityUpKey", "I");
		config.set("GravityDownKey", "K");
		config.set("GravityStep", gravityStep);
		config.save();
	}

	config.load();

	gravityUpKey = config.get<std::string>("GravityUpKey", "I")[0];
	gravityDownKey = config.get<std::string>("GravityDownKey", "K")[0];
	gravityStep = config.get<float>("GravityStep", 25);


	_dispatcher.subscribe(SML::HookLoader::Event::AFGPlayerControllerEnterChatMessage, GetMessage);
	_dispatcher.subscribe(SML::HookLoader::Event::UPlayerInputInputKey, GetInput);
	_dispatcher.subscribe(SML::HookLoader::Event::AWorldSettingsGetGravityZ, GravityMod);

	commandSystem.RegisterCommand("get-gravity", GetGravity);
	commandSystem.RegisterCommand("set-gravity", SetGravity);
}

void FlorensFun::Cleanup() {

}