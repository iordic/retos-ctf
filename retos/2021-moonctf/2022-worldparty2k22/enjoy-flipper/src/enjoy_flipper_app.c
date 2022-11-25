#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <furi.h>
#include <furi_hal.h>


//flagHunters{b92a8905b0bc8950cdfcb58c49de5195}
#define FALSE_FLAG "flagHunters{esta_no_es_la_flag}"
#define FLAG_SIZE 45

typedef struct {
   bool showRealFlag;
   char flag[64];
} State;

typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} AppEvent;


static void render_callback(Canvas* canvas, void* ctx) {
    const State* state = acquire_mutex((ValueMutex*)ctx, 25);
    if(state == NULL) {
        return;
    }
    // border around the edge of the screen
    canvas_draw_frame(canvas, 0, 0, 128, 64);
    canvas_set_font(canvas, FontSecondary);

    canvas_draw_str(canvas, 10, 10, state->flag);

    release_mutex((ValueMutex*)ctx, state);
}

static void input_callback(InputEvent* input_event, FuriMessageQueue* event_queue) {
    furi_assert(event_queue);
    AppEvent event = {.type = EventTypeKey, .input = *input_event};
    furi_message_queue_put(event_queue, &event, 0);
}

void decode_flag(State* state, char *flag) {
    if (state->showRealFlag) {
        for (int i = 0; i < FLAG_SIZE; i++) {
            state->flag[i] = flag[i] + '0';
        }
    } else {
        strncpy(state->flag, FALSE_FLAG, FLAG_SIZE);
    }
}

static void state_init(State* const state) {
    memset(state, 0, sizeof(State));
    state->showRealFlag = true;
}

int32_t enjoy_flipper_app(void* p) {

    char flag[] = {(char) 0x36, (char) 0x3c, (char) 0x31, (char) 0x37, (char) 0x18, 
                   (char) 0x45, (char) 0x3e, (char) 0x44, (char) 0x35, (char) 0x42, 
                   (char) 0x43, (char) 0x4b, (char) 0x32, (char) 0x9,  (char) 0x2, 
                   (char) 0x31, (char) 0x8,  (char) 0x9,  (char) 0x0,  (char) 0x5, 
                   (char) 0x32, (char) 0x0,  (char) 0x32, (char) 0x33, (char) 0x8, 
                   (char) 0x9,  (char) 0x5,  (char) 0x0,  (char) 0x33, (char) 0x34, 
                   (char) 0x36, (char) 0x33, (char) 0x32, (char) 0x5,  (char) 0x8, 
                   (char) 0x33, (char) 0x4,  (char) 0x9,  (char) 0x34, (char) 0x35, 
                   (char) 0x5,  (char) 0x1,  (char) 0x9,  (char) 0x5,  (char) 0x4d};

    UNUSED(p);

    FuriMessageQueue* event_queue = furi_message_queue_alloc(1, sizeof(AppEvent));
    furi_check(event_queue);

    State* state = malloc(sizeof(State));
    state_init(state);

    ValueMutex state_mutex;

    if (!init_mutex(&state_mutex, state, sizeof(State))) {
        free(state); 
        return 255;
    }
    ViewPort* view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, render_callback, &state_mutex);
    view_port_input_callback_set(view_port, input_callback, event_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    AppEvent event;
    bool processing = true;

    while (processing) {
        FuriStatus event_status = furi_message_queue_get(event_queue, &event, 25);
        State* state = (State*)acquire_mutex_block(&state_mutex);

        if(event_status == FuriStatusOk && event.type == EventTypeKey && event.input.type == InputTypePress) {
            if(event.input.key == InputKeyBack) {
                release_mutex(&state_mutex, state);
                processing = false;
                break;
            }   
        }
        decode_flag(state, flag);
        view_port_update(view_port);
        release_mutex(&state_mutex, state);
    }
    view_port_enabled_set(view_port, false);
    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    furi_message_queue_free(event_queue);
    delete_mutex(&state_mutex);
    free(state);
    return 0;
}
