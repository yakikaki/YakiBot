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
#include <dpp/dpp.h>

#include "../../commands/information/about.h"
#include "../../handler/handler.h"

void about(dpp::cluster& client, const dpp::slashcommand_t& event)
{
    // Calculate uptime
    auto uptime = client.uptime();
    auto days = uptime.days;
    auto hours = uptime.hours;
    auto minutes = uptime.mins;
    auto seconds = uptime.secs;

    const auto uptime_str = fmt::format("{}days {}hrs {}mins {}secs", days, hours, minutes, seconds);

    // Get server and member count

    auto about_embed = dpp::embed()
        .set_color(0xB05BB3)
        .add_field("About", fmt::format("{} is a multipurpose Discord bot with plenty of useful systems from ticketing to giveaways. This bot was developed in the hope of making servers manageable by only 1 Discord bot, includes all moderation utilities you'd find in other bots and systems that are very user-friendly.", client.me.username))
        .add_field("Uptime", uptime_str, true)
        .add_field("Developed by", "YakiKaki#2271", true)
        .add_field("Honorable mentions", "[D++](https://dpp.dev)", true)
        .add_field("Support Server", "[Yaki's Studio](https://discord.gg/hFUX6R2kuU)", true)
        .set_footer(dpp::embed_footer().set_text("Written in C++"))
        .set_timestamp(std::time(nullptr));

    event.reply(
        dpp::message(event.command.channel_id, about_embed)
    );
}