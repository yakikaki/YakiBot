#include <spdlog/spdlog.h>
#include <dpp/dpp.h>

#include "../../commands/information/about.h"
#include "../../handler/handler.h"

using json = nlohmann::json;


void about(dpp::cluster& client, const dpp::slashcommand_t& event)
{

    dpp::shard_list shards;

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

    // Calculate uptime
    auto uptime = client.uptime();
    auto days = uptime.days;
    auto hours = uptime.hours;
    auto minutes = uptime.mins;
    auto seconds = uptime.secs;
    
    auto guilds = std::to_string(dpp::get_guild_cache()->count());
    auto users = std::to_string(dpp::get_user_cache()->count());

    auto isBeta = reader["beta"];
    auto Shards = std::to_string(1);


    //emojis
    dpp::emoji dppMoji("dpp", 1195732721037750335);

    const auto uptime_str = fmt::format("{}days {}hrs {}mins {}secs", days, hours, minutes, seconds);

    std::string libraries = "";
    libraries += (dppMoji.get_mention() + " [D++](https://dpp.dev)\n");

    std::string developers = "";
    developers += "YakiKaki\n";

    // Get server and member count
    auto about_embed = dpp::embed()
        .set_color(0xB05BB3)
        .add_field("About", fmt::format("{} is a multipurpose Discord bot with plenty of useful systems from ticketing to giveaways. This bot was developed in the hope of making servers manageable by only 1 Discord bot, includes all moderation utilities you'd find in other bots and systems that are very user-friendly.", client.me.username))
        .add_field("Uptime", uptime_str, true)
        .add_field("Beta Mode", isBeta, true)
        .add_field("Shards", Shards, true)
        .add_field("Servers", guilds, true)
        .add_field("Users", users, true)
        .add_field("Developed by", developers, true)
        .add_field("Libraries", libraries , true)
        .add_field("Support Server", "[Yaki's Studio](https://discord.gg/hFUX6R2kuU)", true)
        .add_field("Website", "[YakiKaki, The site.](https://yakikaki.github.io/YakiBot/Home)", true)
        .set_footer(dpp::embed_footer().set_text("Rights belong to YakiStudios 2022-2024"))
        .set_timestamp(std::time(nullptr));

    event.reply(
        dpp::message(event.command.channel_id, about_embed)
    );
}