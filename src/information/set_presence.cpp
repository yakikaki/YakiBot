#include <spdlog/spdlog.h>

#include "../../commands/information/set_presence.h"
#include "../../handler/handler.h"

void setPresence(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;

	const auto errorTitle = "<:failed:1036206712916553748> Error";
	const auto warnTitle = "Warning message";

	const auto status = event.get_parameter("status");

	if (event.command.usr.id != 299791979514953728ULL)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You don't have permission to change the bot's status", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	client.set_presence(
		dpp::presence(dpp::ps_online, dpp::at_game, "test successful!")
	);

	event.reply(
		dpp::message().set_content("successfuly changed the bot's status!")
		.set_flags(dpp::m_ephemeral)
	);
	
}
