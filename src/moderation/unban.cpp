#include <spdlog/spdlog.h>
#include <dpp/dpp.h>

#include "../../handler/handler.h"
#include "../../handler/btnHandler.h"
#include "../../commands/moderation/unban.h"

void unban(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	dpp::embed embed;

	const auto errorTitle       = "<:failed:1036206712916553748> Error";
	const auto warnTitle        = "Warning message";

	const auto usr              = std::get<dpp::snowflake>(event.get_parameter("member"));

	const auto tgtReason        = event.get_parameter("reason");
	const auto source           = event.command.usr.id;
	const auto tgtGuild         = event.command.guild_id;
	const auto tgtChannel       = event.command.channel_id;
	const auto clientPermission = event.command.app_permissions.has(dpp::p_ban_members);

	const auto gFind            = dpp::find_guild(event.command.guild_id);

	if (gFind == nullptr)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Guild not found!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!gFind->base_permissions(event.command.member).has(dpp::p_ban_members))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to unban", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (!clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to unban", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == gFind->owner_id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Owner never gain a ban", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}
	
	if (usr == source)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You've not been banned before :>", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	if (usr == client.me.id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Well ... have you banned me :(?", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	/*	Making ban map ... will continue to make this in the future!

			client.guild_get_ban(); -> check bans

			client.guild_get_bans(event.command.guild_id, 0, 0, bansLimit, [&client, event](const dpp::confirmation_callback_t& callback)
				{
					// client.guild_ban_delete(); // it should be unbanned in this scope, since we need to unban for who has been banned!
				});

				-> check ban map

			P/S: I'll soon continue to make this, since it is really hard

	*/
	
	auto b_Component   = dpp::component();
	auto cnl_Component = dpp::component();

	b_Component.set_label("Unban").set_type(dpp::cot_button).set_style(dpp::cos_danger).set_emoji("success", 1036206685779398677).set_id("ub_Id");
	cnl_Component.set_label("Cancel").set_type(dpp::cot_button).set_style(dpp::cos_success).set_emoji("failed", 1036206712916553748).set_id("ub_cnl_Id");

	// Button for un-banning
	ButtonBind(b_Component, [&client, tgtGuild, tgtReason, usr, source](const dpp::button_click_t& event)
		{
			// If not the user who request that interaction
			if (source != event.command.usr.id)
				return false;

			const auto  ubContent = fmt::format("Banned removed from <@{}>", usr);
            std::string ub_Reason = "No reason provided";

			// If reason is provided
			if (std::holds_alternative<std::string>(tgtReason))
				ub_Reason = std::get<std::string>(tgtReason);

            client.set_audit_reason(ub_Reason);

			// Unban the user from that guild
			client.guild_ban_delete(event.command.guild_id, usr);

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
				.set_content(ubContent)
			);

			return true;
		});

	// Button for cancelling
	ButtonBind(cnl_Component, [source](const dpp::button_click_t& event)
		{
			// If not the user who request that interaction
			if (source != event.command.usr.id)
				return false;

			const auto cnlContent = "Cancelled request!";

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
				              .set_content(cnlContent)
			);

			return true;
		});

	dpp::message b_Confirm(
		fmt::format("Do you want to unban <@{}>? Press the button below to confirm", usr)
	);

	b_Confirm.add_component(
		dpp::component().add_component(b_Component)
		                .add_component(cnl_Component)
	);

	event.reply(
		b_Confirm.set_flags(dpp::m_ephemeral)
		         .set_channel_id(tgtChannel)
	);
}
