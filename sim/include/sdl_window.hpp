#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <memory>
#include <vector>

struct SdlWindowEventListener {
    virtual void handle_event(const SDL_Event& e) = 0;
    virtual ~SdlWindowEventListener() = default;
};

struct SdlWindowUpdateListener {
    virtual void update() = 0;
    virtual ~SdlWindowUpdateListener() = default;
};

struct SdlWindowRenderListener {
    virtual void render() = 0;
    virtual ~SdlWindowRenderListener() = default;
};

class SdlWindow {
public:
    SdlWindow();
    ~SdlWindow();

    inline SDL_Window* sdl_win() const { return this->m_window; }
    inline SDL_GLContext gl() const { return this->m_gl; }

    void add_event_listener(SdlWindowEventListener* listener);
    void add_update_listener(SdlWindowUpdateListener* listener);
    void add_render_listener(SdlWindowRenderListener* listener);
    void run();

private:
    SDL_Window* m_window;
    SDL_GLContext m_gl;
    std::vector<SdlWindowEventListener*> m_l_evt;
    std::vector<SdlWindowUpdateListener*> m_l_update;
    std::vector<SdlWindowRenderListener*> m_l_render;
};
