#pragma once
#include <functional>
#include <dpp/dpp.h>

void modalHandler(const dpp::form_submit_t& event)
{
	if (event.custom_id == "my_modal")
	{
		std::string v = std::get<std::string>(event.components[0].components[0].value);

		dpp::message m;
		m.set_content("You entered: " + v).set_flags(dpp::m_ephemeral);

		/* Emit a reply. Form submission is still an interaction and must generate some form of reply! */
		event.reply(m);
	}
}