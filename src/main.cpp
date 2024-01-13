#include <fstream>
#include <map>
#include <spdlog/spdlog.h>
#include <dpp/nlohmann/json.hpp>
#include <harshfeudal/harshfeudal.h>
#include <dpp/dpp.h>
#include <Windows.h>

#include "../handler/builder.h"
#include "../handler/btnHandler.h"
#include "../handler/frmHandler.h"
#include "../handler/messageCreateHandler.h"

using json = nlohmann::json;

int main()
{
	int total_shards = 1;
	uint32_t clusterid = 0;
	uint32_t maxclusters = 1;;
	
	json reader;
	{
		std::ifstream reading("config.json");

		if (!reading)
		{
			std::cout << "[ERROR] No \"config.json\" file found!";
			std::cin.get();

			exit(0);
		}
		reading >> reader;
	}

	const auto token = harshfeudal::Base64::Decode(reader["token"]);

	// Client constructor
	dpp::cluster client(reader["token"], dpp::i_all_intents, total_shards, clusterid, maxclusters);

	client.on_ready([&client](const dpp::ready_t& event) 
		{
			// Set presence for the bot
			if (dpp::run_once<struct register_bot_commands>()) {

				client.set_presence(dpp::presence(dpp::presence_status::ps_online, dpp::activity_type::at_custom, "Starting up..."));

				client.start_timer([&client](const dpp::timer& timer) {
					client.set_presence(dpp::presence(dpp::presence_status::ps_online, dpp::activity_type::at_watching, std::to_string(dpp::get_user_cache()->count()) + " members!"));
				}, 120);
			}
			
			// Slash command registration
			SlashCommandCreate(client);

			// Confirm logger
			fmt::print("[{} - STARTED]: {} is online!\n", dpp::utility::current_date_time(), client.me.format_username());
			fmt::print("[{} - REGISTERED]: Successfully registered slash commands!\n", dpp::utility::current_date_time());
			std::cout << "Dpp version: " << dpp::utility::version() << std::endl;
		});


	client.on_message_create([&client](const dpp::message_create_t& event) {
			messageEventHandler(event);
		});

	client.on_button_click([](const dpp::button_click_t& event)
		{
			ButtonHandle(event);
		});

	client.on_form_submit([](const dpp::form_submit_t& event)
		{
			modalHandler(event);
		});

	client.on_slashcommand([&client](const dpp::slashcommand_t& event)
		{
			dpp::command_interaction commandData   = event.command.get_command_interaction();
			auto                     commandFilter = commands.find(commandData.name);

			// Check commands
			if (commandFilter != commands.end())
				commandFilter->second.function(client, event);
		});

	SetConsoleTitle(TEXT("YakiBot - A powerful discord bot"));

	//good for debugging
	 client.on_log(dpp::utility::cout_logger());

	client.start(dpp::st_wait);
	
	return 0;
}


