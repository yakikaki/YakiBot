#include <spdlog/spdlog.h>
#include <dpp/dpp.h>

#include "../../commands/utility/Modal.h"
#include "../../handler/handler.h"

void Modal(dpp::cluster& client, const dpp::slashcommand_t& event)
{
    dpp::interaction_modal_response modal("my_modal", "Please enter stuff");

    /* Add a text component */
    modal.add_component(
        dpp::component()
        .set_label("Short type rammel")
        .set_id("field_id")
        .set_type(dpp::cot_text)
        .set_placeholder("gumd")
        .set_min_length(5)
        .set_max_length(50)
        .set_text_style(dpp::text_short)
    );

    /* Add another text component in the next row, as required by Discord */
    modal.add_row();
    modal.add_component(
        dpp::component()
        .set_label("Type rammel")
        .set_id("field_id2")
        .set_type(dpp::cot_text)
        .set_placeholder("gumf")
        .set_min_length(1)
        .set_max_length(2000)
        .set_text_style(dpp::text_paragraph)
    );

    /* Trigger the dialog box. All dialog boxes are ephemeral */
    event.dialog(modal);
}
