#include "raylib.h"
#include "BST.h"
#include "FileManager.h"
#include "MergeSort.h"

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ==========================
// Layout Constants
// ==========================
static const int SCREEN_WIDTH  = 1000;
static const int SCREEN_HEIGHT = 700;

static const Rectangle PANEL_RECT = { 360, 120, 600, 560 };

static const int CARD_HEIGHT = 86;

// ==========================
// Helper: Text measurement with a Font
// ==========================
float TextW(const Font &font, const string &text, float fontSize)
{
    return MeasureTextEx(font, text.c_str(), fontSize, 1.0f).x;
}

// ==========================
// Helper: Draw Left Buttons
// ==========================
void DrawButtons(const Font &font, Rectangle addBtn, Rectangle displayBtn, Rectangle searchBtn,
                  Rectangle updateBtn, Rectangle deleteBtn, Rectangle sortBtn,
                  Rectangle exitBtn)
{
    DrawRectangleRec(addBtn, SKYBLUE);
    DrawRectangleRec(displayBtn, SKYBLUE);
    DrawRectangleRec(searchBtn, SKYBLUE);
    DrawRectangleRec(updateBtn, SKYBLUE);
    DrawRectangleRec(deleteBtn, SKYBLUE);
    DrawRectangleRec(sortBtn, SKYBLUE);
    DrawRectangleRec(exitBtn, RED);

    DrawTextEx(font, "Add Hospital",      { 120, 135 }, 20, 1.0f, BLACK);
    DrawTextEx(font, "Display Hospitals", { 100, 205 }, 20, 1.0f, BLACK);
    DrawTextEx(font, "Search Hospital",   { 105, 275 }, 20, 1.0f, BLACK);
    DrawTextEx(font, "Update ICU Beds",   { 105, 345 }, 20, 1.0f, BLACK);
    DrawTextEx(font, "Delete Hospital",   { 105, 415 }, 20, 1.0f, BLACK);
    DrawTextEx(font, "Sort by ICU Beds",  { 100, 485 }, 20, 1.0f, BLACK);
    DrawTextEx(font, "Exit",              { 170, 555 }, 20, 1.0f, WHITE);
}

// ==========================
// Helper: Draw Panel Header
// ==========================
void DrawPanelHeader(const Font &font, const string &title)
{
    DrawRectangleLines((int)PANEL_RECT.x, (int)PANEL_RECT.y,
                        (int)PANEL_RECT.width, (int)PANEL_RECT.height, BLACK);

    float textWidth = TextW(font, title, 24);
    float centerX = PANEL_RECT.x + PANEL_RECT.width / 2 - textWidth / 2;
    DrawTextEx(font, title.c_str(), { centerX, 135 }, 24, 1.0f, DARKBLUE);
}

// ==========================
// Helper: Draw Status Message
// ==========================
void DrawStatusMessage(const Font &font, const string &statusMessage)
{
    if (!statusMessage.empty())
    {
        DrawTextEx(font, statusMessage.c_str(), { 360, 90 }, 22, 1.0f, MAROON);
    }
}

// ==========================
// Helper: Draw Hospital List (scrolling + scrollbar)
// ==========================
void DrawHospitalList(const Font &font, const vector<Hospital> &hospitals, float scrollOffset)
{
    BeginScissorMode((int)PANEL_RECT.x + 1, (int)PANEL_RECT.y + 41,
                      (int)PANEL_RECT.width - 2, (int)PANEL_RECT.height - 42);

    int startY = (int)PANEL_RECT.y + 50;
    int y = startY + (int)scrollOffset;

    for (const Hospital &h : hospitals)
    {
        if (y + CARD_HEIGHT >= (int)PANEL_RECT.y + 41 && y <= (int)(PANEL_RECT.y + PANEL_RECT.height))
        {
            DrawTextEx(font, ("ID : " + to_string(h.id)).c_str(),
                       { 390, (float)y }, 18, 1.0f, DARKBLUE);

            DrawTextEx(font, h.name.c_str(),
                       { 390, (float)(y + 20) }, 18, 1.0f, BLACK);

            DrawTextEx(font, h.location.c_str(),
                       { 390, (float)(y + 40) }, 18, 1.0f, DARKGRAY);

            DrawTextEx(font, ("ICU Beds : " + to_string(h.icuBeds)).c_str(),
                       { 390, (float)(y + 60) }, 18, 1.0f, RED);

            DrawLine(390, y + 82, 910, y + 82, LIGHTGRAY);
        }

        y += CARD_HEIGHT;
    }

    if (hospitals.empty())
    {
        DrawTextEx(font, "No hospitals to display.", { 480, 300 }, 20, 1.0f, GRAY);
    }

    EndScissorMode();

    // ---- Simple scrollbar ----
    float visibleHeight = PANEL_RECT.height - 42;
    float contentHeight = (float)hospitals.size() * CARD_HEIGHT;

    if (contentHeight > visibleHeight)
    {
        float trackX = PANEL_RECT.x + PANEL_RECT.width - 12;
        float trackY = PANEL_RECT.y + 41;

        DrawRectangle((int)trackX, (int)trackY, 6, (int)visibleHeight, Fade(LIGHTGRAY, 0.5f));

        float thumbHeight = visibleHeight * (visibleHeight / contentHeight);
        if (thumbHeight < 24) thumbHeight = 24;

        float maxScroll = contentHeight - visibleHeight;
        float scrollRatio = maxScroll > 0 ? (-scrollOffset) / maxScroll : 0.0f;

        float thumbY = trackY + scrollRatio * (visibleHeight - thumbHeight);

        DrawRectangle((int)trackX, (int)thumbY, 6, (int)thumbHeight, DARKBLUE);
    }
}

// ==========================
// Helper: Draw Search Result Card
// ==========================
void DrawSearchResultCard(const Font &font, bool hospitalFoundFlag, const Hospital &searchedHospital)
{
    float cardWidth  = 420;
    float cardHeight = 220;
    float cardX = PANEL_RECT.x + (PANEL_RECT.width - cardWidth) / 2;
    float cardY = PANEL_RECT.y + 70;

    if (hospitalFoundFlag)
    {
        DrawRectangle((int)cardX, (int)cardY, (int)cardWidth, (int)cardHeight, Fade(SKYBLUE, 0.2f));
        DrawRectangleLines((int)cardX, (int)cardY, (int)cardWidth, (int)cardHeight, DARKBLUE);

        string header = "Hospital Found!";
        float headerW = TextW(font, header, 22);
        DrawTextEx(font, header.c_str(), { cardX + cardWidth / 2 - headerW / 2, cardY + 15 }, 22, 1.0f, DARKGREEN);

        float textX = cardX + 30;
        float textY = cardY + 55;

        DrawTextEx(font, ("ID       : " + to_string(searchedHospital.id)).c_str(),
                   { textX, textY }, 20, 1.0f, BLACK);

        DrawTextEx(font, ("Name     : " + searchedHospital.name).c_str(),
                   { textX, textY + 35 }, 20, 1.0f, BLACK);

        DrawTextEx(font, ("Location : " + searchedHospital.location).c_str(),
                   { textX, textY + 70 }, 20, 1.0f, BLACK);

        DrawTextEx(font, ("ICU Beds : " + to_string(searchedHospital.icuBeds)).c_str(),
                   { textX, textY + 105 }, 20, 1.0f, RED);
    }
    else
    {
        string msg = "Hospital Not Found!";
        float fontSize = 28;
        float textWidth = TextW(font, msg, fontSize);

        float centerX = PANEL_RECT.x + PANEL_RECT.width / 2 - textWidth / 2;
        float centerY = cardY + cardHeight / 2 - fontSize / 2;

        DrawTextEx(font, msg.c_str(), { centerX, centerY }, fontSize, 1.0f, RED);
    }

    Rectangle backBtn = { PANEL_RECT.x + PANEL_RECT.width / 2 - 70, cardY + cardHeight + 20, 140, 40 };
    DrawRectangleRec(backBtn, LIGHTGRAY);
    DrawRectangleLinesEx(backBtn, 2, GRAY);
    float bw = TextW(font, "Back", 18);
    DrawTextEx(font, "Back", { backBtn.x + backBtn.width / 2 - bw / 2, backBtn.y + 11 }, 18, 1.0f, BLACK);
}

// ==========================
// Helper: Draw a text input box
// ==========================
void DrawFormBox(const Font &font, const string &label, const string &value, Rectangle box, bool active)
{
    DrawTextEx(font, label.c_str(), { box.x, box.y - 24 }, 18, 1.0f, DARKBLUE);

    DrawRectangleRec(box, RAYWHITE);
    DrawRectangleLinesEx(box, active ? 3 : 2, active ? DARKBLUE : BLACK);

    DrawTextEx(font, value.c_str(), { box.x + 10, box.y + 11 }, 18, 1.0f, BLACK);

    if (active && ((int)(GetTime() * 2.0)) % 2 == 0)
    {
        float textWidth = TextW(font, value, 18);
        DrawTextEx(font, "|", { box.x + 14 + textWidth, box.y + 10 }, 18, 1.0f, DARKBLUE);
    }
}

// ==========================
// Helper: Draw a form action button (Save/Search/Update/Delete/Cancel)
// ==========================
void DrawActionButton(const Font &font, Rectangle box, const string &label, Color bg, Color border)
{
    DrawRectangleRec(box, bg);
    DrawRectangleLinesEx(box, 2, border);
    float w = TextW(font, label, 18);
    DrawTextEx(font, label.c_str(), { box.x + box.width / 2 - w / 2, box.y + 13 }, 18, 1.0f, BLACK);
}

// ==========================
// Helper: Append typed character with optional numeric restriction
// ==========================
void AppendChar(string &field, int key, bool numericOnly, size_t maxLen)
{
    bool valid = (key >= 32) && (key <= 125);

    if (valid && numericOnly)
    {
        valid = (key >= '0' && key <= '9');
    }

    if (valid && field.size() < maxLen)
    {
        field += (char)key;
    }
}

// ==========================
// Helper: Safe integer parsing
// ==========================
bool TryParseInt(const string &text, int &outValue)
{
    if (text.empty()) return false;

    try
    {
        outValue = stoi(text);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

// ==========================
// Helper: Reset all form/result flags to false
// ==========================
void ResetAllForms(bool &showAddForm, bool &showSearchForm, bool &showUpdateForm,
                    bool &showDeleteForm, bool &showSearchResult)
{
    showAddForm = false;
    showSearchForm = false;
    showUpdateForm = false;
    showDeleteForm = false;
    showSearchResult = false;
}

// ==========================
// Helper: Clamp Scroll Offset
// ==========================
float ClampScroll(float scrollOffset, size_t itemCount)
{
    float visibleHeight = PANEL_RECT.height - 42;
    float contentHeight = (float)itemCount * CARD_HEIGHT;

    float minOffset = visibleHeight - contentHeight;
    if (minOffset > 0) minOffset = 0;

    if (scrollOffset > 0) scrollOffset = 0;
    if (scrollOffset < minOffset) scrollOffset = minOffset;

    return scrollOffset;
}

int main()
{
    BST tree;
    FileManager fileManager;
    MergeSort sorter;

    fileManager.loadHospitals(tree);

    vector<Hospital> hospitals = tree.getAllHospitals();

    // ==========================
    // GUI State (declared before the main loop)
    // ==========================
    bool showAddForm = false;
    bool showSearchForm = false;
    bool showUpdateForm = false;
    bool showDeleteForm = false;

    bool showSearchResult = false;
    bool hospitalFoundFlag = false;

    bool isSorted = false;

    string statusMessage = "";

    string inputID = "";
    string inputName = "";
    string inputLocation = "";
    string inputICU = "";
    int addActiveField = 0; // 0=ID, 1=Name, 2=Location, 3=ICU

    string searchID = "";

    string updateID = "";
    string updateBeds = "";
    int updateActiveField = 0; // 0=ID, 1=Beds

    string deleteID = "";

    Hospital searchedHospital;

    float scrollOffset = 0.0f;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "HealthRoute - Admin Panel");
    SetTargetFPS(60);

    // ---- Custom font with safe fallback ----
    // LoadFontEx with a larger base size (rather than plain LoadFont) keeps
    // text crisp at every size used in this GUI (16px status text up to the
    // 40px title) instead of upscaling a small baked texture and looking
    // pixelated.
    bool usingCustomFont = FileExists("assets/font.ttf");
    Font appFont = usingCustomFont ? LoadFontEx("assets/font.ttf", 96, NULL, 0) : GetFontDefault();

    if (usingCustomFont && appFont.texture.id == 0)
    {
        // Load failed despite the file existing — fall back safely.
        appFont = GetFontDefault();
        usingCustomFont = false;
    }

    if (usingCustomFont)
    {
        SetTextureFilter(appFont.texture, TEXTURE_FILTER_BILINEAR);
    }

    // ---- Left menu buttons ----
    Rectangle addBtn     = { 70,120,250,50 };
    Rectangle displayBtn = { 70,190,250,50 };
    Rectangle searchBtn  = { 70,260,250,50 };
    Rectangle updateBtn  = { 70,330,250,50 };
    Rectangle deleteBtn  = { 70,400,250,50 };
    Rectangle sortBtn    = { 70,470,250,50 };
    Rectangle exitBtn    = { 70,540,250,50 };

    // ---- Add Hospital form layout ----
    Rectangle idBox        = { 500, 190, 320, 40 };
    Rectangle nameBox      = { 500, 260, 320, 40 };
    Rectangle locationBox  = { 500, 330, 320, 40 };
    Rectangle icuBox       = { 500, 400, 320, 40 };
    Rectangle addSubmitBtn = { 500, 470, 140, 45 };
    Rectangle addCancelBtn = { 660, 470, 140, 45 };

    // ---- Search Hospital form layout ----
    Rectangle searchIdBox    = { 500, 210, 320, 40 };
    Rectangle searchGoBtn    = { 500, 280, 140, 45 };
    Rectangle searchCancelBtn= { 660, 280, 140, 45 };

    // ---- Update ICU Beds form layout ----
    Rectangle updateIdBox    = { 500, 210, 320, 40 };
    Rectangle updateBedsBox  = { 500, 290, 320, 40 };
    Rectangle updateGoBtn    = { 500, 360, 140, 45 };
    Rectangle updateCancelBtn= { 660, 360, 140, 45 };

    // ---- Delete Hospital form layout ----
    Rectangle deleteIdBox    = { 500, 210, 320, 40 };
    Rectangle deleteGoBtn    = { 500, 280, 140, 45 };
    Rectangle deleteCancelBtn= { 660, 280, 140, 45 };

    while (!WindowShouldClose())
    {
        Vector2 mouse = GetMousePosition();

        // ==========================
        // Keyboard input for whichever form is open
        // ==========================
        if (showAddForm)
        {
            if (IsKeyPressed(KEY_TAB)) addActiveField = (addActiveField + 1) % 4;

            int key = GetCharPressed();
            while (key > 0)
            {
                bool numeric = (addActiveField == 0 || addActiveField == 3);
                size_t maxLen = numeric ? 10 : 40;

                switch (addActiveField)
                {
                    case 0: AppendChar(inputID, key, numeric, maxLen); break;
                    case 1: AppendChar(inputName, key, numeric, maxLen); break;
                    case 2: AppendChar(inputLocation, key, numeric, maxLen); break;
                    case 3: AppendChar(inputICU, key, numeric, maxLen); break;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                switch (addActiveField)
                {
                    case 0: if (!inputID.empty()) inputID.pop_back(); break;
                    case 1: if (!inputName.empty()) inputName.pop_back(); break;
                    case 2: if (!inputLocation.empty()) inputLocation.pop_back(); break;
                    case 3: if (!inputICU.empty()) inputICU.pop_back(); break;
                }
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                if (addActiveField < 3) addActiveField++;
            }
        }
        else if (showSearchForm)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                AppendChar(searchID, key, true, 10);
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !searchID.empty())
            {
                searchID.pop_back();
            }
        }
        else if (showUpdateForm)
        {
            if (IsKeyPressed(KEY_TAB)) updateActiveField = (updateActiveField + 1) % 2;

            int key = GetCharPressed();
            while (key > 0)
            {
                if (updateActiveField == 0) AppendChar(updateID, key, true, 10);
                else AppendChar(updateBeds, key, true, 6);
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                if (updateActiveField == 0) { if (!updateID.empty()) updateID.pop_back(); }
                else { if (!updateBeds.empty()) updateBeds.pop_back(); }
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                if (updateActiveField == 0) updateActiveField = 1;
            }
        }
        else if (showDeleteForm)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                AppendChar(deleteID, key, true, 10);
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !deleteID.empty())
            {
                deleteID.pop_back();
            }
        }

        // ==========================
        // Scroll input (only when a plain list is showing)
        // ==========================
        bool anyFormOpen = showAddForm || showSearchForm || showUpdateForm || showDeleteForm || showSearchResult;

        if (!anyFormOpen && CheckCollisionPointRec(mouse, PANEL_RECT))
        {
            scrollOffset += GetMouseWheelMove() * 25.0f;
            scrollOffset = ClampScroll(scrollOffset, hospitals.size());
        }

        // ==========================
        // Mouse Click Handling
        // ==========================
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // ---- Left menu (always available; switches mode and closes other forms) ----
            if (CheckCollisionPointRec(mouse, addBtn))
            {
                ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                showAddForm = true;
                inputID = inputName = inputLocation = inputICU = "";
                addActiveField = 0;
                statusMessage = "";
            }

            if (CheckCollisionPointRec(mouse, displayBtn))
            {
                ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                hospitals = tree.getAllHospitals();
                isSorted = false;
                scrollOffset = 0.0f;
                statusMessage = "Displaying Hospitals";
            }

            if (CheckCollisionPointRec(mouse, searchBtn))
            {
                ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                showSearchForm = true;
                searchID = "";
                statusMessage = "";
            }

            if (CheckCollisionPointRec(mouse, updateBtn))
            {
                ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                showUpdateForm = true;
                updateID = updateBeds = "";
                updateActiveField = 0;
                statusMessage = "";
            }

            if (CheckCollisionPointRec(mouse, deleteBtn))
            {
                ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                showDeleteForm = true;
                deleteID = "";
                statusMessage = "";
            }

            if (CheckCollisionPointRec(mouse, sortBtn))
            {
                ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                hospitals = tree.getAllHospitals();

                if (!hospitals.empty())
                {
                    sorter.mergeSort(hospitals, 0, (int)hospitals.size() - 1);
                }

                isSorted = true;
                scrollOffset = 0.0f;
                statusMessage = "Hospitals Sorted Successfully!";
            }

            if (CheckCollisionPointRec(mouse, exitBtn))
            {
                if (usingCustomFont) UnloadFont(appFont);
                CloseWindow();
                return 0;
            }

            // ---- ADD FORM ----
            if (showAddForm)
            {
                if (CheckCollisionPointRec(mouse, idBox))       addActiveField = 0;
                if (CheckCollisionPointRec(mouse, nameBox))     addActiveField = 1;
                if (CheckCollisionPointRec(mouse, locationBox)) addActiveField = 2;
                if (CheckCollisionPointRec(mouse, icuBox))      addActiveField = 3;

                if (CheckCollisionPointRec(mouse, addSubmitBtn))
                {
                    if (inputID.empty() || inputName.empty() || inputLocation.empty() || inputICU.empty())
                    {
                        statusMessage = "Please fill all fields!";
                    }
                    else
                    {
                        int id = 0, icuBeds = 0;

                        if (!TryParseInt(inputID, id) || !TryParseInt(inputICU, icuBeds))
                        {
                            statusMessage = "Hospital ID and ICU Beds must be valid numbers!";
                        }
                        else
                        {
                            hospitals = tree.getAllHospitals();
                            bool duplicate = false;
                            for (const Hospital &h : hospitals)
                            {
                                if (h.id == id) { duplicate = true; break; }
                            }

                            if (duplicate)
                            {
                                statusMessage = "Hospital ID already exists!";
                            }
                            else
                            {
                                Hospital h(id, inputName, inputLocation, icuBeds);
                                tree.insert(h);

                                hospitals = tree.getAllHospitals();
                                fileManager.saveHospitals(hospitals);

                                ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                                isSorted = false;
                                scrollOffset = 0.0f;
                                statusMessage = "Hospital Added Successfully!";
                            }
                        }
                    }
                }

                if (CheckCollisionPointRec(mouse, addCancelBtn))
                {
                    ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                    statusMessage = "";
                }
            }

            // ---- SEARCH FORM ----
            if (showSearchForm)
            {
                if (CheckCollisionPointRec(mouse, searchIdBox)) { /* focus (only field) */ }

                if (CheckCollisionPointRec(mouse, searchGoBtn))
                {
                    int id = 0;

                    if (!TryParseInt(searchID, id))
                    {
                        statusMessage = "Please enter a valid Hospital ID!";
                    }
                    else
                    {
                        hospitals = tree.getAllHospitals();
                        hospitalFoundFlag = false;

                        for (const Hospital &h : hospitals)
                        {
                            if (h.id == id)
                            {
                                searchedHospital = h;
                                hospitalFoundFlag = true;
                                break;
                            }
                        }

                        showSearchForm = false;
                        showSearchResult = true;
                        statusMessage = hospitalFoundFlag ? "Hospital Found!" : "Hospital Not Found!";
                    }
                }

                if (CheckCollisionPointRec(mouse, searchCancelBtn))
                {
                    ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                    statusMessage = "";
                }
            }

            // ---- SEARCH RESULT ----
            if (showSearchResult)
            {
                Rectangle backBtn = { PANEL_RECT.x + PANEL_RECT.width / 2 - 70, PANEL_RECT.y + 70 + 220 + 20, 140, 40 };
                if (CheckCollisionPointRec(mouse, backBtn))
                {
                    ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                    statusMessage = "";
                }
            }

            // ---- UPDATE FORM ----
            if (showUpdateForm)
            {
                if (CheckCollisionPointRec(mouse, updateIdBox))   updateActiveField = 0;
                if (CheckCollisionPointRec(mouse, updateBedsBox)) updateActiveField = 1;

                if (CheckCollisionPointRec(mouse, updateGoBtn))
                {
                    int id = 0, newBeds = 0;

                    if (!TryParseInt(updateID, id) || !TryParseInt(updateBeds, newBeds))
                    {
                        statusMessage = "Please enter valid numbers!";
                    }
                    else
                    {
                        hospitals = tree.getAllHospitals();
                        bool exists = false;
                        for (const Hospital &h : hospitals)
                        {
                            if (h.id == id) { exists = true; break; }
                        }

                        if (!exists)
                        {
                            statusMessage = "Hospital Not Found!";
                        }
                        else
                        {
                            tree.updateICUBeds(id, newBeds);

                            hospitals = tree.getAllHospitals();
                            fileManager.saveHospitals(hospitals);

                            ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                            isSorted = false;
                            scrollOffset = 0.0f;
                            statusMessage = "Hospital Updated Successfully!";
                        }
                    }
                }

                if (CheckCollisionPointRec(mouse, updateCancelBtn))
                {
                    ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                    statusMessage = "";
                }
            }

            // ---- DELETE FORM ----
            if (showDeleteForm)
            {
                if (CheckCollisionPointRec(mouse, deleteIdBox)) { /* focus (only field) */ }

                if (CheckCollisionPointRec(mouse, deleteGoBtn))
                {
                    int id = 0;

                    if (!TryParseInt(deleteID, id))
                    {
                        statusMessage = "Please enter a valid Hospital ID!";
                    }
                    else
                    {
                        hospitals = tree.getAllHospitals();
                        bool exists = false;
                        for (const Hospital &h : hospitals)
                        {
                            if (h.id == id) { exists = true; break; }
                        }

                        if (!exists)
                        {
                            statusMessage = "Hospital Not Found!";
                        }
                        else
                        {
                            tree.deleteHospital(id);

                            hospitals = tree.getAllHospitals();
                            fileManager.saveHospitals(hospitals);

                            ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                            isSorted = false;
                            scrollOffset = 0.0f;
                            statusMessage = "Hospital Deleted Successfully!";
                        }
                    }
                }

                if (CheckCollisionPointRec(mouse, deleteCancelBtn))
                {
                    ResetAllForms(showAddForm, showSearchForm, showUpdateForm, showDeleteForm, showSearchResult);
                    statusMessage = "";
                }
            }
        }

        // ==========================
        // Drawing
        // ==========================
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextEx(appFont, "HEALTHROUTE ADMIN PANEL", { 250, 30 }, 40, 1.0f, DARKBLUE);

        DrawStatusMessage(appFont, statusMessage);

        DrawButtons(appFont, addBtn, displayBtn, searchBtn, updateBtn, deleteBtn, sortBtn, exitBtn);

        string panelTitle;
        if (showAddForm)            panelTitle = "Add Hospital";
        else if (showSearchForm)    panelTitle = "Search Hospital";
        else if (showUpdateForm)    panelTitle = "Update ICU Beds";
        else if (showDeleteForm)    panelTitle = "Delete Hospital";
        else if (showSearchResult)  panelTitle = "Search Result";
        else if (isSorted)          panelTitle = "Sorted By ICU Beds (Highest First)";
        else                        panelTitle = "Hospital Records";

        DrawPanelHeader(appFont, panelTitle);

        if (showAddForm)
        {
            DrawFormBox(appFont, "Hospital ID:", inputID, idBox, addActiveField == 0);
            DrawFormBox(appFont, "Name:", inputName, nameBox, addActiveField == 1);
            DrawFormBox(appFont, "Location:", inputLocation, locationBox, addActiveField == 2);
            DrawFormBox(appFont, "ICU Beds:", inputICU, icuBox, addActiveField == 3);

            DrawActionButton(appFont, addSubmitBtn, "ADD", SKYBLUE, DARKBLUE);
            DrawActionButton(appFont, addCancelBtn, "CANCEL", LIGHTGRAY, GRAY);
        }
        else if (showSearchForm)
        {
            DrawFormBox(appFont, "Hospital ID:", searchID, searchIdBox, true);

            DrawActionButton(appFont, searchGoBtn, "SEARCH", SKYBLUE, DARKBLUE);
            DrawActionButton(appFont, searchCancelBtn, "CANCEL", LIGHTGRAY, GRAY);
        }
        else if (showSearchResult)
        {
            DrawSearchResultCard(appFont, hospitalFoundFlag, searchedHospital);
        }
        else if (showUpdateForm)
        {
            DrawFormBox(appFont, "Hospital ID:", updateID, updateIdBox, updateActiveField == 0);
            DrawFormBox(appFont, "New ICU Beds:", updateBeds, updateBedsBox, updateActiveField == 1);

            DrawActionButton(appFont, updateGoBtn, "UPDATE", SKYBLUE, DARKBLUE);
            DrawActionButton(appFont, updateCancelBtn, "CANCEL", LIGHTGRAY, GRAY);
        }
        else if (showDeleteForm)
        {
            DrawFormBox(appFont, "Hospital ID:", deleteID, deleteIdBox, true);

            DrawActionButton(appFont, deleteGoBtn, "DELETE", SKYBLUE, DARKBLUE);
            DrawActionButton(appFont, deleteCancelBtn, "CANCEL", LIGHTGRAY, GRAY);
        }
        else
        {
            DrawHospitalList(appFont, hospitals, scrollOffset);
        }

        EndDrawing();
    }

    if (usingCustomFont) UnloadFont(appFont);

    CloseWindow();

    return 0;
}