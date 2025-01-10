#include "MenuManager.h"

MenuManager::MenuManager() {

    mainMenuTexture = { 0 };
    pauseMenuTexture = { 0 };
    gameOverMenuTexture = { 0 };
    map1Preview = { 0 };
    map2Preview = { 0 };
    character1Texture = { 0 };
    character2Texture = { 0 };
    character3Texture = { 0 };
    character4Texture = { 0 };
    character5Texture = { 0 };
}

MenuManager::~MenuManager() {
    CleanUp();
}

void MenuManager::LoadAssets() {

    mainMenuTexture = LoadTexture("Assets/menuprincipal.png");
    pauseMenuTexture = LoadTexture("Assets/pausemenu.png");
    gameOverMenuTexture = LoadTexture("Assets/gameover.png");
    Win = LoadTexture("Assets/WinScreen.png");
    instructions = LoadTexture("Assets/instructions.png");
    map1Preview = LoadTexture("Assets/map1_preview.png");
    map2Preview = LoadTexture("Assets/map2_preview.png");
    map1Full = LoadTexture("Assets/map2.png");
    map2Full = LoadTexture("Assets/map_hielo.png");

	map1Background = LoadTexture("Assets/Map1_fondo.png");
	map2Background = LoadTexture("Assets/Map2_fondo.png");

    character1Texture = LoadTexture("Assets/Car.png");
    character2Texture = LoadTexture("Assets/Car2.png");
    character3Texture = LoadTexture("Assets/Car3.png");
    character4Texture = LoadTexture("Assets/Car4.png");
    character5Texture = LoadTexture("Assets/Car5.png");

    NormalMode = LoadTexture("Assets/normalmode.png");
    GreenRedMode = LoadTexture("Assets/greenredmode.png");
}

void MenuManager::DrawMainMenu() {
    DrawTexture(mainMenuTexture, 0, 0, WHITE);
}

void MenuManager::DrawCharacterSelectMenu(int selectedCharacter) {

   
    const int startX = GetScreenWidth() / 2 - 300;
    const int startY = GetScreenHeight() / 2 - character1Texture.height / 2;
    const int offsetX = 150; 

   
    DrawTexture(character1Texture, startX, startY, WHITE);
    DrawTexture(character2Texture, startX + offsetX, startY, WHITE);
    DrawTexture(character3Texture, startX + 2 * offsetX, startY, WHITE);
    DrawTexture(character4Texture, startX + 3 * offsetX, startY, WHITE);
    DrawTexture(character5Texture, startX + 4 * offsetX, startY, WHITE);

   
    if (selectedCharacter >= 0 && selectedCharacter < 5) {
        int highlightX = startX + selectedCharacter * offsetX;
        DrawRectangleLines(highlightX - 10, startY - 10, character1Texture.width + 20, character1Texture.height + 20, YELLOW);
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

void MenuManager::DrawGameModeSelectionMenu(int selectedMode)
{
    DrawTexture(NormalMode, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - map1Preview.height / 2, WHITE);
    DrawTexture(GreenRedMode, GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 - map2Preview.height / 2, WHITE);
    if (selectedMode == 0) {


        DrawTexture(NormalMode, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - map1Preview.height / 2, WHITE);
        DrawTexture(GreenRedMode, GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 - map2Preview.height / 2, WHITE);

        DrawRectangleLines(GetScreenWidth() / 2 - 410, GetScreenHeight() / 2 - map1Preview.height / 2 - 10,
            map1Preview.width + 20, map1Preview.height + 20, YELLOW);
    }
    else if(selectedMode == 1) {


        DrawTexture(NormalMode, GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - map1Preview.height / 2, WHITE);
        DrawTexture(GreenRedMode, GetScreenWidth() / 2 + 100, GetScreenHeight() / 2 - map2Preview.height / 2, WHITE);

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

void MenuManager::DrawWinMenu()
{
    DrawTexture(Win, 0, 0, WHITE);
}

void MenuManager::DrawInstructions()
{
    DrawTexture(instructions, 0, 0, WHITE);
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
    UnloadTexture(character3Texture);
    UnloadTexture(character4Texture);
    UnloadTexture(character5Texture);
}

Texture2D MenuManager::GetCharacter1Texture() const
{
    return character1Texture;
}

Texture2D MenuManager::GetCharacter2Texture() const
{
    return character2Texture;
}

Texture2D MenuManager::GetCharacter3Texture() const
{
    return character3Texture;
}

Texture2D MenuManager::GetCharacter4Texture() const
{
    return character4Texture;
}

Texture2D MenuManager::GetCharacter5Texture() const
{
    return character5Texture;
}

Texture2D MenuManager::GetMap1Full() const
{
    return map1Full;
}

Texture2D MenuManager::GetMap2Full() const
{
    return map2Full;
}
