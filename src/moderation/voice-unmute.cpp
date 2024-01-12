/*
 * Copyright 2022 harshfeudal and The Harshfeudal Projects contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <spdlog/spdlog.h>

#include "../../commands/moderation/voice-unmute.h"
#include "../../handler/handler.h"
#include "../../handler/btnHandler.h"

void unmute(dpp::cluster& client, const dpp::slashcommand_t& event)
{
	const auto  errorTitle       = "<:failed:1036206712916553748> Error";
	const auto  warnTitle        = "Warning message";

	const auto  usr              = std::get<dpp::snowflake>(event.get_parameter("user"));
	const auto  gFind            = dpp::find_guild(event.command.guild_id);

	const auto  tgtReason        = event.get_parameter("reason");
	const auto  source           = event.command.usr.id;
	const auto  tgtGuild         = event.command.guild_id;
	const auto  tgtChannel       = event.command.channel_id;
	const auto  clientPermission = event.command.app_permissions.has(dpp::p_mute_members);

	const auto  tgtUser          = gFind->members.find(usr);
	
	dpp::embed  embed;
	dpp::guild* Guild           = dpp::find_guild(tgtGuild);

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
	if (!gFind->base_permissions(event.command.member).has(dpp::p_mute_members))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You have lack of permission to unmute member", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If the bot doesn't have any permission
	if (!clientPermission)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "I have lack of permission to unmute member", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If they try to unmute a guild owner
	if (usr == gFind->owner_id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot unmute the owner", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If they unmute theirselves
	if (usr == source)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "You cannot unmute yourself", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	// If they try to unmute the bot
	if (usr == client.me.id)
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "Thanks, but I am immune :>", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

    // If the user not in the voice channel
	if (!Guild->connect_member_voice(usr))
	{
		EmbedBuild(embed, 0xFF7578, errorTitle, warnTitle, "User is not in the voice channel!", event.command.usr);
		event.reply(
			dpp::message(event.command.channel_id, embed).set_flags(dpp::m_ephemeral)
		);

		return;
	}

	auto unmute_Component = dpp::component();
	auto cnl_Component    = dpp::component();

	unmute_Component.set_label("Unmute").set_type(dpp::cot_button).set_style(dpp::cos_danger).set_emoji("success", 1036206685779398677).set_id("d_Id");
	cnl_Component.set_label("Cancel").set_type(dpp::cot_button).set_style(dpp::cos_success).set_emoji("failed", 1036206712916553748).set_id("d_cnl_Id");

	// Button for muting
	ButtonBind(unmute_Component, [&client, tgtGuild, tgtReason, usr, source](const dpp::button_click_t& event)
		{
			// If not the user who request that interaction
			if (source != event.command.usr.id)
				return false;

			const auto      mContent       = fmt::format("<@{}> has been unmuted!", usr);
			auto            TargetVoiceMem = dpp::find_guild_member(tgtGuild, usr);

			TargetVoiceMem.set_mute(false);
			client.guild_edit_member(TargetVoiceMem);

			event.reply(
				dpp::interaction_response_type::ir_update_message,
				dpp::message().set_flags(dpp::m_ephemeral)
							  .set_content(mContent)
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

	dpp::message unmute_Confirm(
		fmt::format("Do you want to unmute <@{}>? Press the button below to confirm", usr)
	);

	unmute_Confirm.add_component(
		dpp::component().add_component(unmute_Component)
		                .add_component(cnl_Component)
	);

	event.reply(
		unmute_Confirm.set_flags(dpp::m_ephemeral)
		         .set_channel_id(tgtChannel)
	);
}
