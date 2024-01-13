#include <spdlog/spdlog.h>

#include "../../commands/moderation/nickname.h"
#include "../../handler/handler.h"

void nickname(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;

	const auto errorTitle       = "<:failed:1036206712916553748> Error";
	const auto warnTitle        = "Warning message";

	const auto usr              = std::get<dpp::snowflake>(event.get_parameter("target"));
	const auto gFind            = dpp::find_guild(event.command.guild_id);

	const auto setNickname      = event.get_parameter("nickname");
	const auto tgtGuild         = event.command.guild_id;
	const auto tgtChannel       = event.command.channel_id;
	const auto clientPermission = event.command.app_permissions.has(dpp::p_manage_nicknames);

	const auto tgtUser          = gFind->members.find(usr);

	auto getNicknameEditUsr     = dpp::find_guild_member(event.command.guild_id, usr);

	// If cannot find that member in the server
	if (tgtUser == gFind->members.end())
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Member not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If cannot find the guild to action
	if (gFind == nullptr)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Guild not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If the command user doesn't have any permission
	if (!gFind->base_permissions(event.command.member).has(dpp::p_manage_nicknames))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to change nickname", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If the bot doesn't have any permission
	if (!clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to change nickname", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If they try to change nickname a guild owner
	if (event.command.usr.id != gFind->owner_id && usr == gFind->owner_id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot change or clear nickname from the owner", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	std::string       NicknameStatus;

	const std::string SelfNickStatus = fmt::format("You have {} your nickname", NicknameStatus     );
	const auto        announce       = fmt::format("Nickname {} from <@{}>!",   NicknameStatus, usr);

	std::string       ReplyContent   = announce;

	// If the nickname is set or clear
	if (std::holds_alternative<std::string>(setNickname) == true)
	{
		client.guild_edit_member(
			getNicknameEditUsr.set_nickname(std::get<std::string>(setNickname))
		);

		NicknameStatus = "changed";

		if (event.command.usr.id == usr)
			ReplyContent = SelfNickStatus;

		event.reply(
			dpp::message().set_content(ReplyContent)
			              .set_flags(dpp::m_ephemeral)
		);
	}
	else
	{
		client.guild_edit_member(
			getNicknameEditUsr.set_nickname(dpp::find_user(usr)->username)
		);

		NicknameStatus = "cleared";

		if (event.command.usr.id == usr)
			ReplyContent = SelfNickStatus;

		event.reply(
			dpp::message().set_content(ReplyContent)
			              .set_flags(dpp::m_ephemeral)
		);
	}
}
