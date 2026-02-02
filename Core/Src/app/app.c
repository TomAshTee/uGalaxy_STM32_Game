/**
 * @file    app.c
 * @author  Tomasz
 * @date    Jan 23, 2026
 * @brief   Application layer implementation.
 *
 * This module glues together:
 * - input sampling,
 * - game logic execution,
 * - OLED rendering.
 *
 * It is driven by two tick flags:
 * - logicTick  -> one logic step
 * - renderTick -> one render step (only when OLED DMA is free)
 */

#include "app/app.h"

#include "drivers/ssd1327.h"
#include "gfx/GFX_ssd1327.h"
#include "gfx/graphics_map.h"
#include <string.h>

/** ##########################################################################
 *  @name Local functions
 *  @brief Internal state handlers.
 *  @{
 *  ##########################################################################
 */
static void RunMenu(InputSnapshot *in);
static void RunMenuTick(AppCtx *app, InputSnapshot *in);

static void RunGame(InputSnapshot *in, GameCtx *g);
static void RunGameTick(InputSnapshot *in, GameCtx *g);

static void RunDead(InputSnapshot *in, GameCtx *g);
static void RunDeadTick(AppCtx *app, InputSnapshot *in);

static void PlayDeadAnim(void);

/** @} */

/** ##########################################################################
 *  @name Functions
 *  @brief Public API.
 *  @{
 *  ##########################################################################
 */

void App_Init(AppCtx *app, GameCtx *game, SPI_HandleTypeDef *hspi,
		ADC_HandleTypeDef *hadc, GPIO_TypeDef *btn1Port, uint16_t btn1Pin,
		GPIO_TypeDef *btn2Port, uint16_t btn2Pin) {
	if (app == NULL) {
		return;
	}

	memset(app, 0, sizeof(*app));
	app->game = game;

	/* OLED init */
	SSD1327_SpiInit(hspi);
	SSD1327_BeginFrame();
	SSD1327_Display();

	/* Game + Input init */
	if (app->game != NULL) {
		GameInit(app->game);
	}

	InputInit(hadc, btn1Port, btn1Pin, btn2Port, btn2Pin);
}

void App_RunOnce(AppCtx *app) {
	if ((app == NULL) || (app->game == NULL)) {
		return;
	}

	/* ---------------- Logic update ---------------- */
	if (app->logicTick) {
		app->logicTick = false;
		app->input = InputRead();

		switch (GameGetState(app->game)) {
		case GS_Menu:
			RunMenuTick(app, &app->input);
			break;

		case GS_Playing:
			RunGameTick(&app->input, app->game);
			break;

		case GS_Dead:
			RunDeadTick(app, &app->input);
			break;

		default:
			break;
		}
	}

	/* ---------------- Render update ---------------- */
	if (app->renderTick && !SSD1327_IsBusy()) {
		app->renderTick = false;
		SSD1327_BeginFrame();

		switch (GameGetState(app->game)) {
		case GS_Menu:
			RunMenu(&app->input);
			break;

		case GS_Playing:
			RunGame(&app->input, app->game);
			break;

		case GS_Dead:
			RunDead(&app->input, app->game);
			break;

		default:
			break;
		}

		SSD1327_Present();
	}
}

void App_OnTimerTick(AppCtx *app) {
	if (app == NULL) {
		return;
	}
	app->logicTick = true;
	app->renderTick = true;
}

/** @} */

/** ##########################################################################
 *  @name Local functions
 *  @brief State handlers (draw/tick).
 *  @{
 *  ##########################################################################
 */

static void RunDead(InputSnapshot *in, GameCtx *g) {
	static int x = 0, dx = 1;
	x += dx;
	if (x < 1 || x > 55) {
		dx = -dx;
	}

	GFX_DrawBitMap_P(x, (SCREEN_HEIGHT / 2) - 4, Defeated_map, 67, 16, 1);
	GFX_DrawBitMap_P(35, (SCREEN_HEIGHT / 2) + 24, Score_map, 37, 10, 1);
	GFX_PutInt(73, (SCREEN_HEIGHT / 2) + 27, GameGetPlayerScore(g), 1, 1, 0);
}

static void RunDeadTick(AppCtx *app, InputSnapshot *in) {
	if (in->btn1State == GPIO_PIN_SET) {
		PlayDeadAnim();
		GameSetState(app->game, GS_Menu);
	}
}

static void PlayDeadAnim(void) {
	uint8_t i;

	for (i = 0; i < 10; ++i) {
		SSD1327_BeginFrame();
		GFX_FillRect(0, 0, 128, 128, 1);
		SSD1327_Display();
		HAL_Delay(10);

		SSD1327_BeginFrame();
		SSD1327_Display();
		HAL_Delay(10);
	}
}

static void RunGame(InputSnapshot *in, GameCtx *g) {
	GameDraw(g, in);
}

static void RunGameTick(InputSnapshot *in, GameCtx *g) {
	//GameLevelUpdate(g);
	GameTick(g, in);
	// GameUpdateBackground(g);
	// GameUpdateBonus(g);
	// GameUpdateExplosion(g);
}

static void RunMenu(InputSnapshot *in) {
	static int x = 0, dx = 1;
	x += dx;
	if (x < 1 || x > 65) {
		dx = -dx;
	}

	GFX_DrawBitMap_P(x, (SCREEN_HEIGHT / 2) - 10, uGalaxy_map, 54, 16, 1);
	GFX_DrawRoundRect(15, (SCREEN_HEIGHT / 2) + 34, 93, 20, 8, 1);
	GFX_DrawBitMap_P(26, (SCREEN_HEIGHT / 2) + 37, PressToStart_map, 66, 10, 1);
}

static void RunMenuTick(AppCtx *app, InputSnapshot *in) {
	if (in->btn1State == GPIO_PIN_SET) {
		GameInit(app->game);
		GameSetState(app->game, GS_Playing);
	}
}

/** @} */
