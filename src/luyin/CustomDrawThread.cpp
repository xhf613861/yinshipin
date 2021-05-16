#include "CustomDrawThread.h"

#include <SDL2/SDL.h>

#include <QDebug>

#define END(judge, func) \
    if (judge) { \
        qDebug() << #func << "Error" << SDL_GetError(); \
        goto end; \
    }

CustomDrawThread::CustomDrawThread(QObject *parent) : QThread(parent)
{
    connect(this, &CustomDrawThread::finished,
            this, &CustomDrawThread::deleteLater);
}

CustomDrawThread::~CustomDrawThread()
{
    requestInterruption();
    quit();
    wait();

    qDebug() << "CustomDrawThread xigou";
}

void CustomDrawThread::run()
{
    // 窗口
    SDL_Window *window = nullptr;
    // 渲染上下文
    SDL_Renderer *renderer = nullptr;
    // 像素数据
    SDL_Surface *surface = nullptr;
    // 纹理（直接跟特定驱动程序相关的像素数据）
    SDL_Texture *texture = nullptr;
    // 事件
    SDL_Event event;

    SDL_Rect src = {0, 0, 50, 50};
    SDL_Rect dst = {0, 0, 50, 50};

    // 初始化Video 子系统
    END(SDL_Init(SDL_INIT_VIDEO), SDL_Init);

    // 创建窗口
    window = SDL_CreateWindow(
                // 窗口标题
                "SDL显示BMP图片",
                // 窗口X
                SDL_WINDOWPOS_UNDEFINED,
                // 窗口Y
                SDL_WINDOWPOS_UNDEFINED,
                // 窗口宽度（跟图片宽度一样）
                500,
                // 窗口高度（跟图片高度一样）
                500,
                // 显示窗口
                SDL_WINDOW_SHOWN);
    END(!window, SDL_CreateWindow);

    // 创建渲染上下文（默认的渲染目标是window）
    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    END(!renderer, SDL_CreateRenderer);

    // 创建纹理
    texture = createTexture(renderer);
    END(!texture, createTexture);

    END(SDL_SetRenderTarget(renderer, nullptr), SDL_SetRenderTarget);

    // 设置绘制颜色（治理随便设置了一个染色：黄色）
    END(SDL_SetRenderDrawColor(renderer, 255, 255, 9, SDL_ALPHA_OPAQUE), SDL_SetRenderDrawColor);

    // 用DrawColor清除渲染目标
    END(SDL_RenderClear(renderer), SDL_RenderClear);

    // 复制纹理到渲染目标上
    END(SDL_RenderCopy(renderer, texture, nullptr, &dst), SDL_RenderCopy);

    // 将此前的所有需要渲染的内容更新到屏幕上
    SDL_RenderPresent(renderer);

    // 监听退出时间
    while (!isInterruptionRequested())
    {
        END(!SDL_WaitEvent(&event), SDL_WaitEvent);

        if (event.type == SDL_QUIT)
            break;
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            // 用DrawColor清除渲染目标
            END(SDL_RenderClear(renderer), SDL_RenderClear);

            // 复制纹理到渲染目标上
            dst = {event.button.x - 25, event.button.y - 25, 50, 50};
            END(SDL_RenderCopy(renderer, texture, nullptr, &dst), SDL_RenderCopy);
            // 将此前的所有需要渲染的内容更新到屏幕上
            SDL_RenderPresent(renderer);
        }
    }

end:
    // 释放资源
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture *CustomDrawThread::createTexture(SDL_Renderer *rendere)
{
    SDL_Texture *texture = SDL_CreateTexture(rendere, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, 50, 50);
    if (texture == nullptr)
        return nullptr;

    if (SDL_SetRenderTarget(rendere, texture))
        return nullptr;

    if (SDL_SetRenderDrawColor(rendere, 0, 255, 0, SDL_ALPHA_OPAQUE))
        return nullptr;

    SDL_Rect draw = {0, 0, 50, 50};
    SDL_RenderDrawRect(rendere, &draw);
    SDL_RenderDrawLine(rendere, 0, 0, 50, 50);
    SDL_RenderDrawLine(rendere, 50, 0, 0, 50);

    return texture;
}
