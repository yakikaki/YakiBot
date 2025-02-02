#pragma once

#include <map>

#include "handler.h"

#include "../commands/moderation/kick.h"
#include "../commands/moderation/ban.h"
#include "../commands/moderation/unban.h"
#include "../commands/moderation/prune.h"
#include "../commands/moderation/timeout.h"
#include "../commands/moderation/nickname.h"
#include "../commands/moderation/disconnect.h"
#include "../commands/moderation/move.h"
#include "../commands/moderation/deafen.h"
#include "../commands/moderation/undeafen.h"
#include "../commands/moderation/voice-mute.h"
#include "../commands/moderation/voice-unmute.h"

#include "../commands/information/ping.h"
#include "../commands/information/userinfo.h"
#include "../commands/information/invite.h"
#include "../commands/information/about.h"
#include "../commands/information/set_presence.h"

#include "../commands/utility/Modal.h"


void SlashCommandCreate(dpp::cluster& client);

// Slash coomand builder
inline std::map<std::string, commandDef> commands
{
	{
		"ping", { "Check bot latency", ping }
	},
	{
		"modal", { "Modal Test", Modal }
	},

	{
		"invite", { "Get the bot's invite link", invite }
	},

	{
		"kick",
			{
				"Kick a member you mentioned", kick,
				{
					dpp::command_option(dpp::co_user,   "member", "Mention a member to kick", true),
					dpp::command_option(dpp::co_string, "reason", "Reason why they got kick", false)
				}
			}
	},
	{
		"ban",
			{
				"Ban a member you mentioned", ban,
				{
					dpp::command_option(dpp::co_user,   "member", "Mention a member to ban", true),
					dpp::command_option(dpp::co_string, "reason", "Reason why they got ban", false)
				}
			}
	},
	{
		"unban",
			{
				"Unban a member you mentioned", unban,
				{
					dpp::command_option(dpp::co_user,   "member", "Mention a member to unban",  true),
					dpp::command_option(dpp::co_string, "reason", "Reason why they remove ban", false)
				}
			}
	},
	{
		"userinfo",
			{
				"Show mentioned user info", infouser,
				{
					dpp::command_option(dpp::co_user, "user", "User you would like to know", false)
				}
			}
	},
	{
		"prune",
			{
				"Prune messages", prune,
				{
					dpp::command_option(dpp::co_integer, "amount", "Amount of messages to prune, from 2 up to 99", true)
				}
			}
	},
	{
		"about",
			{
				"The bot's credits", about
			}
	},
	{
		"timeout",
			{
				"Timeout member", timeout,
				{
					dpp::command_option(dpp::co_user,    "member",   "Mention a member to timeout", true),
					dpp::command_option(dpp::co_string,  "duration", "Timeout duration",            true),
					dpp::command_option(dpp::co_string,  "reason",   "Reason why they got timeout", false)
				}
			}
	},
    {
        "disconnect",
            {
                "Disconnect a user in the voice call", disconnect,
                {
                    dpp::command_option(dpp::co_user,   "user",   "User that you want to disconnect",  true),
                    dpp::command_option(dpp::co_string, "reason", "Reason why they gain a disconnect", false)
                }
            }
    },
	{
        "move",
            {
                "Move a user to another voice call", move,
                {
                    dpp::command_option(dpp::co_user,    "user",    "User that you want to move",  true),
                    dpp::command_option(dpp::co_channel, "channel", "New voice channel",           true),
                    dpp::command_option(dpp::co_string,  "reason",  "Reason why they gain a move", false)
                }
            }
    },
	{
        "deafen",
            {
                "Deafen a user in the voice call", deafen,
                {
                    dpp::command_option(dpp::co_user,  "user",   "User that you want to deafen",  true)
                }
            }
    },
	{
        "undeafen",
            {
                "Undeafen a user in the voice call", undeafen,
                {
                    dpp::command_option(dpp::co_user,  "user",   "User that you want to undeafen",  true)
                }
            }
    },
	{
        "voice-mute",
            {
                "Mute a user in the voice call", mute,
                {
                    dpp::command_option(dpp::co_user,  "user",   "User that you want to mute",  true)
                }
            }
    },
	{
        "voice-unmute",
            {
                "Unmute a user in the voice call", unmute,
                {
                    dpp::command_option(dpp::co_user,  "user",   "User that you want to unmute",  true)
                }
            }
    },
	{
		"nick",
			{
				"Change member nickname or reset to their original username", nickname,
				{
					dpp::command_option(dpp::co_user,   "target",   "the member you're trying to rename",                  true),
					dpp::command_option(dpp::co_string, "nickname", "Set their new nickname, leave empty to reset",        false)
				}
			}
	},

	{
		"set-presence",
		{
			"Change the bot's presence", setPresence,
			{
				dpp::command_option(dpp::co_string, "status", "The new presence", false)
			}
		}
	}
};

inline void SlashCommandCreate(dpp::cluster& client)
{
	if (dpp::run_once<struct register_commands>())
	{
		std::vector<dpp::slashcommand> slashCmds;

		for (auto& def : commands)
		{
			dpp::slashcommand cmd;

			// Create slash command template
			cmd.set_name(def.first)
			   .set_description(def.second.description)
			   .set_application_id(client.me.id);

			cmd.options = def.second.param;

			// Pushing all commands
			slashCmds.push_back(cmd);
		}

		// Create a global slash commands
		client.global_bulk_command_create(slashCmds);
		client.guild_bulk_command_create({ }, 796876694333292554);
	}
}
