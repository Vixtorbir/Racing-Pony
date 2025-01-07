#include "MenuManager.h"

MenuManager::MenuManager() {

    mainMenuTexture = { 0 };
    pauseMenuTexture = { 0 };
    gameOverMenuTexture = { 0 };
    map1Preview = { 0 };
    map2Preview = { 0 };
    character1Texture = { 0 };
    character2Texture = { 0 };
}

MenuManager::~MenuManager() {
    CleanUp();
}

void MenuManager::LoadAssets() {

    mainMenuTexture = LoadTexture("Assets/menuprincipal.png");
    pauseMenuTexture = LoadTexture("Assets/pausemenu.png");
    gameOverMenuTexture = LoadTexture("Assets/gameovermenu.png");

    map1Preview = LoadTexture("Assets/map1_preview.png");
    map2Preview = LoadTexture("Assets/map2_preview.png");
    map1Full = LoadTexture("Assets/map2.png");
    map2Full = LoadTexture("Assets/map_hielo.png");

	map1Background = LoadTexture("Assets/Map1_fondo.png");
	map2Background = LoadTexture("Assets/Map2_fondo.png");

    character1Texture = LoadTexture("Assets/CarUsable.png");
    character2Texture = LoadTexture("Assets/Car2.png");
}

void MenuManager::DrawMainMenu() {
    DrawTexture(mainMenuTexture, 0, 0, WHITE);
}

void MenuManager::DrawCharacterSelectMenu(int selectedCharacter) {

    DrawTexture(character1Texture, GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 - character1Texture.height / 2, WHITE);
    DrawTexture(character2Texture, GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 - character2Texture.height / 2, WHITE);

    if (selectedCharacter == 0) {
        DrawRectangleLines(GetScreenWidth() / 2 - 210, GetScreenHeight() / 2 - character1Texture.height / 2 - 10,
            character1Texture.width + 20, character1Texture.height + 20, YELLOW);
    }
    else {
        DrawRectangleLines(GetScreenWidth() / 2 + 90, GetScreenHeight() / 2 - character2Texture.height / 2 - 10,
            character2Texture.width + 20, character2Texture.height + 20, YELLOW);
    }
}

void MenuManager::DrawMapSelectMenu(int selectedMap) {

    DrawTexture(map1Preview, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - map1Preview.height / 2, WHITE);
    DrawTexture(map2Preview, GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 - map2Preview.height / 2, WHITE);

    if (selectedMap == 0) {

		DrawTexture(map1Background, 0, 0, WHITE);

        DrawTexture(map1Preview, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - map1Preview.height / 2, WHITE);
        DrawTexture(map2Preview, GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 - map2Preview.height / 2, WHITE);

        DrawRectangleLines(GetScreenWidth() / 2 - 410, GetScreenHeight() / 2 - map1Preview.height / 2 - 10,
            map1Preview.width + 20, map1Preview.height + 20, YELLOW);
    }
    else {

		DrawTexture(map2Background, 0, 0, WHITE);

        DrawTexture(map1Preview, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - map1Preview.height / 2, WHITE);
        DrawTexture(map2Preview, GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 - map2Preview.height / 2, WHITE);

        DrawRectangleLines(GetScreenWidth() / 2 + 90, GetScreenHeight() / 2 - map2Preview.height / 2 - 10,
            map2Preview.width + 20, map2Preview.height + 20, YELLOW);
    }
}

void MenuManager::DrawPauseMenu() {
    DrawTexture(pauseMenuTexture, 0, 0, WHITE);
}

void MenuManager::DrawGameOverMenu() {
    DrawTexture(gameOverMenuTexture, 0, 0, WHITE);
}

void MenuManager::CleanUp() {
    UnloadTexture(mainMenuTexture);
    UnloadTexture(pauseMenuTexture);
    UnloadTexture(gameOverMenuTexture);
    UnloadTexture(map1Preview);
    UnloadTexture(map2Preview);
    UnloadTexture(map1Full);
    UnloadTexture(map2Full);
    UnloadTexture(character1Texture);
    UnloadTexture(character2Texture);
}

Texture2D MenuManager::GetCharacter1Texture() const
{
    return character1Texture;
}

Texture2D MenuManager::GetCharacter2Texture() const
{
    return character2Texture;
}

Texture2D MenuManager::GetMap1Full() const
{
    return map1Full;
}

Texture2D MenuManager::GetMap2Full() const
{
    return map2Full;
}
