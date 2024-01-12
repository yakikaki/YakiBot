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

#pragma once

#include <dpp/dpp.h>

inline void EmbedInfoBuild(dpp::embed& embed, const dpp::user& tgtUser)
{
    embed = dpp::embed();
    const auto invUrl = fmt::format("https://discord.com/api/oauth2/authorize?client_id=831629627507736606&permissions=8&scope=bot%20applications.commands");
    embed.set_color(0xAA7EEE)
        .set_title("YakiBot Invite Link")
        .add_field("Invite Yaki bot to your server!", fmt::format("Click the button below to [invite]({}) the bot to your server.", invUrl), true)
        .set_footer(dpp::embed_footer().set_text(tgtUser.username).set_icon(tgtUser.get_avatar_url()))
        .set_timestamp(time(nullptr));
}

void invite(dpp::cluster& client, const dpp::slashcommand_t& event);
