#include <3ds.h>

#include "action.h"
#include "../../error.h"
#include "../../progressbar.h"
#include "../../prompt.h"

static void action_delete_ticket_success_onresponse(ui_view* view, void* data, bool response) {
    task_refresh_tickets();

    prompt_destroy(view);
}

static void action_delete_ticket_update(ui_view* view, void* data, float* progress, char* progressText) {
    ticket_info* info = (ticket_info*) data;

    Result res = AM_DeleteTicket(info->ticketId);
    if(R_FAILED(res)) {
        progressbar_destroy(view);
        ui_pop();

        error_display_res(info, ui_draw_ticket_info, res, "Failed to delete ticket.");

        return;
    }

    progressbar_destroy(view);
    ui_pop();

    ui_push(prompt_create("Success", "Ticket deleted.", 0xFF000000, false, info, NULL, ui_draw_ticket_info, action_delete_ticket_success_onresponse));
}

static void action_delete_ticket_onresponse(ui_view* view, void* data, bool response) {
    if(response) {
        prompt_destroy(view);

        ui_push(progressbar_create("Deleting Ticket", "", data, action_delete_ticket_update, ui_draw_ticket_info));
    }
}

void action_delete_ticket(ticket_info* info) {
    ui_push(prompt_create("Confirmation", "Delete the selected ticket?", 0xFF000000, true, info, NULL, ui_draw_ticket_info, action_delete_ticket_onresponse));
}