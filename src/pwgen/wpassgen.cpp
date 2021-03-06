/*******************************************************************************
 * src/pwgen/wpassgen.cpp
 *
 * Part of CryptoTE v0.5.999, see http://panthema.net/2007/cryptote
 *******************************************************************************
 * Copyright (C) 2008-2014 Timo Bingmann <tb@panthema.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307 USA
 ******************************************************************************/

#include "wpassgen.h"

#include "common/tools.h"
#include "fips181.h"
#include "enctain.h"

#include <wx/config.h>
#include <wx/textdlg.h>
#include <wx/tokenzr.h>

// begin wxGlade: ::extracode
// end wxGlade

WPassGen::WPassGen(wxWindow* parent, bool _standalone, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long WXUNUSED(style))
    : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxTHICK_FRAME),
      standalone(_standalone)
{
    {
        #include "art/pwgen-16.h"
        #include "art/pwgen-22.h"
        #include "art/pwgen-32.h"
        #include "art/pwgen-48.h"

        wxIconBundle progicon;

        progicon.AddIcon(wxIconFromMemory(pwgen_16_png));
        progicon.AddIcon(wxIconFromMemory(pwgen_22_png));
        progicon.AddIcon(wxIconFromMemory(pwgen_32_png));
        progicon.AddIcon(wxIconFromMemory(pwgen_48_png));

        SetIcons(progicon);
    }

    // begin wxGlade: WPassGen::WPassGen
    sizer2_staticbox = new wxStaticBox(this, -1, _("Generator Options"));
    const wxString* choicePreset_choices = NULL;
    choicePreset = new wxChoice(this, myID_PRESET, wxDefaultPosition, wxDefaultSize, 0, choicePreset_choices, 0);
    buttonPresetAdd = new wxBitmapButton(this, myID_PRESET_ADD, wxNullBitmap);
    buttonPresetRemove = new wxBitmapButton(this, myID_PRESET_REMOVE, wxNullBitmap);
    const wxString* choiceType_choices = NULL;
    choiceType = new wxChoice(this, myID_TYPE, wxDefaultPosition, wxDefaultSize, 0, choiceType_choices, 0);
    checkboxSkipSimilarChar = new wxCheckBox(this, myID_SKIPSIMILARCHAR, _("Don't use 0/O and 1/l."));
    checkboxSkipSwappedChar = new wxCheckBox(this, myID_SKIPSWAPPEDCHAR, _("Don't use z/y."));
    textctrlExtraChar = new wxTextCtrl(this, myID_TEXT_EXTRACHAR, wxEmptyString);
    spinctrlLength = new wxSpinCtrl(this, myID_LENGTH, wxT("12"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 6, 100);
    textctrlStrength = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    spinctrlNumber = new wxSpinCtrl(this, myID_NUMBER, wxT("10"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 1000);
    checkboxEnumerate = new wxCheckBox(this, myID_ENUMERATE, _("Enumerate"));
    buttonGenerate = new wxButton(this, myID_GENERATE, _("&Generate"));
    listctrlPasslist = new wxListCtrl(this, myID_PASSLIST, wxDefaultPosition, wxDefaultSize, wxLC_LIST | wxSUNKEN_BORDER);
    textctrlPasslist = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    buttonOK = new wxButton(this, wxID_OK, wxEmptyString);
    buttonCancel = new wxButton(this, wxID_CANCEL, wxEmptyString);
    buttonAbout = new wxButton(this, wxID_ABOUT, _("&About"));
    buttonClose = new wxButton(this, wxID_CLOSE, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

    // set bitmap from compiled-in PNGs
    {
        #include "art/gnome/list-add.h"
        #include "art/gnome/list-remove.h"

        buttonPresetAdd->SetBitmapLabel(wxBitmapFromMemory(gnome_list_add_png));
        buttonPresetRemove->SetBitmapLabel(wxBitmapFromMemory(gnome_list_remove_png));
    }

    // insert password type names
    for (unsigned int pt = 0; pt <= PT_LAST; ++pt)
    {
        choiceType->Append(GetTypeName((pass_type)pt));
    }

    // update dialog logic
    ResetPresetChoice();
    UpdateCheckboxes();
    UpdateKeyStrength();
    GenerateList();

    buttonOK->Disable();
}

void WPassGen::set_properties()
{
    // begin wxGlade: WPassGen::set_properties
    SetTitle(_("CryptoTE Password Generator"));
    SetSize(wxSize(502, 600));
    choicePreset->SetToolTip(_("Default or saved password generation presets."));
    buttonPresetAdd->SetToolTip(_("Save the current settings as a new preset."));
    buttonPresetAdd->SetSize(buttonPresetAdd->GetBestSize());
    buttonPresetRemove->SetToolTip(_("Remove current preset."));
    buttonPresetRemove->SetSize(buttonPresetRemove->GetBestSize());
    choiceType->SetToolTip(_("Select desired password components."));
    checkboxSkipSimilarChar->SetToolTip(_("Does not use 0/O and 1/l, because these can hardly be distinguished in some fonts."));
    checkboxSkipSwappedChar->SetToolTip(_("Does not include characters in password which are swapped on some keyboards."));
    textctrlExtraChar->SetToolTip(_("Extra non-standard characters to include in the base random set."));
    spinctrlLength->SetToolTip(_("Password length desired."));
    textctrlStrength->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    textctrlStrength->SetToolTip(_("Theoretic number of keybits the entropy of the selected pass type generates."));
    spinctrlNumber->SetToolTip(_("Number of passwords to generate."));
    checkboxEnumerate->SetToolTip(_("Add serial enumeration to each password line."));
    listctrlPasslist->SetFont(wxFont(10, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));
    textctrlPasslist->SetFont(wxFont(10, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));
    // end wxGlade
}

void WPassGen::do_layout()
{
    // begin wxGlade: WPassGen::do_layout
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* sizer7 = new wxGridSizer(1, 2, 0, 0);
    wxGridSizer* sizer6 = new wxGridSizer(1, 2, 0, 0);
    wxStaticBoxSizer* sizer2 = new wxStaticBoxSizer(sizer2_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer3 = new wxFlexGridSizer(8, 2, 0, 0);
    wxBoxSizer* sizer5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer4 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* label1 = new wxStaticText(this, wxID_ANY, _("Preset:"));
    sizer3->Add(label1, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer4->Add(choicePreset, 1, wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 6);
    sizer4->Add(buttonPresetAdd, 0, wxLEFT, 6);
    sizer4->Add(buttonPresetRemove, 0, wxLEFT, 6);
    sizer3->Add(sizer4, 1, wxALL | wxEXPAND, 6);
    wxStaticText* label2 = new wxStaticText(this, wxID_ANY, _("Type:"));
    sizer3->Add(label2, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(choiceType, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(5, 5, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(checkboxSkipSimilarChar, 0, wxLEFT | wxRIGHT | wxBOTTOM, 6);
    sizer3->Add(5, 5, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(checkboxSkipSwappedChar, 0, wxLEFT | wxRIGHT | wxBOTTOM, 6);
    wxStaticText* label3 = new wxStaticText(this, wxID_ANY, _("Extra Characters:"));
    sizer3->Add(label3, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(textctrlExtraChar, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND | wxALIGN_CENTER_VERTICAL, 6);
    wxStaticText* label4 = new wxStaticText(this, wxID_ANY, _("Length:"));
    sizer3->Add(label4, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(spinctrlLength, 0, wxLEFT | wxRIGHT | wxBOTTOM, 6);
    wxStaticText* label5 = new wxStaticText(this, wxID_ANY, _("Theoretic Strength:"));
    sizer3->Add(label5, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(textctrlStrength, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND | wxALIGN_CENTER_VERTICAL, 6);
    wxStaticText* label6 = new wxStaticText(this, wxID_ANY, _("Number of Passwords:"));
    sizer3->Add(label6, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer5->Add(spinctrlNumber, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 6);
    sizer5->Add(checkboxEnumerate, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(sizer5, 1, wxEXPAND, 0);
    sizer3->AddGrowableCol(1);
    sizer2->Add(sizer3, 1, wxEXPAND, 0);
    sizer2->Add(buttonGenerate, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 6);
    sizer1->Add(sizer2, 0, wxALL | wxEXPAND, 8);
    wxStaticLine* staticline1 = new wxStaticLine(this, wxID_ANY);
    sizer1->Add(staticline1, 0, wxEXPAND, 0);
    sizer1->Add(listctrlPasslist, 1, wxALL | wxEXPAND, 8);
    sizer1->Add(textctrlPasslist, 1, wxALL | wxEXPAND, 8);
    wxStaticLine* staticline2 = new wxStaticLine(this, wxID_ANY);
    sizer1->Add(staticline2, 0, wxEXPAND, 0);
    sizer6->Add(buttonOK, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer6->Add(buttonCancel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 6);
    sizer1->Add(sizer6, 0, wxEXPAND, 0);
    sizer7->Add(buttonAbout, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer7->Add(buttonClose, 0, wxALL | wxALIGN_CENTER_VERTICAL, 6);
    sizer1->Add(sizer7, 0, wxEXPAND, 0);
    SetSizer(sizer1);
    Layout();
    Centre();
    // end wxGlade

    if (standalone)
    {
        sizer1->Hide(listctrlPasslist);
        sizer1->Hide(sizer6);
    }
    else
    {
        sizer5->Hide(checkboxEnumerate);
        sizer1->Hide(textctrlPasslist);
        sizer1->Hide(sizer7);
    }
}

BEGIN_EVENT_TABLE(WPassGen, wxDialog)
// begin wxGlade: WPassGen::event_table
    EVT_CHOICE(myID_PRESET, WPassGen::OnChoicePreset)
    EVT_BUTTON(myID_PRESET_ADD, WPassGen::OnButtonPresetAdd)
    EVT_BUTTON(myID_PRESET_REMOVE, WPassGen::OnButtonPresetRemove)
    EVT_CHOICE(myID_TYPE, WPassGen::OnChoiceType)
    EVT_CHECKBOX(myID_SKIPSIMILARCHAR, WPassGen::OnCheckSkipSimilarChar)
    EVT_CHECKBOX(myID_SKIPSWAPPEDCHAR, WPassGen::OnCheckSkipSwappedChar)
    EVT_TEXT(myID_TEXT_EXTRACHAR, WPassGen::OnTextExtraCharChange)
    EVT_SPINCTRL(myID_LENGTH, WPassGen::OnSpinLength)
    EVT_SPINCTRL(myID_NUMBER, WPassGen::OnSpinNumber)
    EVT_CHECKBOX(myID_ENUMERATE, WPassGen::OnCheckEnumerate)
    EVT_BUTTON(myID_GENERATE, WPassGen::OnButtonGenerate)
    EVT_LIST_ITEM_DESELECTED(myID_PASSLIST, WPassGen::OnPasslistSelected)
    EVT_LIST_ITEM_SELECTED(myID_PASSLIST, WPassGen::OnPasslistSelected)
    EVT_LIST_ITEM_ACTIVATED(myID_PASSLIST, WPassGen::OnPasslistActivated)
    EVT_BUTTON(wxID_OK, WPassGen::OnButtonOK)
    EVT_BUTTON(wxID_CANCEL, WPassGen::OnButtonCancel)
    EVT_BUTTON(wxID_ABOUT, WPassGen::OnButtonAbout)
    EVT_BUTTON(wxID_ANY, WPassGen::OnButtonClose)
// end wxGlade
    EVT_CLOSE(WPassGen::OnClose)
END_EVENT_TABLE();

const std::vector<WPassGen::Preset>& WPassGen::GetDefaultPresets()
{
    static std::vector<Preset> default_presets;

    if (default_presets.empty())
    {
        default_presets.push_back(Preset(_("8 Char AlPhANuM"),
                                         PT_ALPHANUMERIC, false, false, _T(""), 8));

        default_presets.push_back(Preset(_("12 Char AlPhANuM"),
                                         PT_ALPHANUMERIC, false, false, _T(""), 12));

        default_presets.push_back(Preset(_("12 Char alpha"),
                                         PT_ALPHALOWER, false, false, _T(""), 12));

        default_presets.push_back(Preset(_("12 Letter Pronounceable"),
                                         PT_PRONOUNCEABLE, false, false, _T(""), 12));

        default_presets.push_back(Preset(_("16 Char AlPhANuM"),
                                         PT_ALPHANUMERIC, false, false, _T(""), 16));

        default_presets.push_back(Preset(_("16 Char alpha"),
                                         PT_ALPHALOWER, false, false, _T(""), 16));

        default_presets.push_back(Preset(_("16 Letter Pronounceable"),
                                         PT_PRONOUNCEABLE, false, false, _T(""), 16));

        default_presets.push_back(Preset(_("128 Keybits AlPhANuM"),
                                         PT_ALPHANUMERIC, false, false, _T(""), 22));

        default_presets.push_back(Preset(_("256 Keybits AlPhANuM"),
                                         PT_ALPHANUMERIC, false, false, _T(""), 43));

        default_presets.push_back(Preset(_("IP Port Number"),
                                         PT_PORTNUMBER, false, false, _T(""), 5));

        default_presets.push_back(Preset(_("64-bit WEP Key (40 keybits)"),
                                         PT_HEXADECIMAL, false, false, _T(""), 10));

        default_presets.push_back(Preset(_("128-bit WEP Key (104 keybits)"),
                                         PT_HEXADECIMAL, false, false, _T(""), 26));

        default_presets.push_back(Preset(_("256-bit WEP Key (232 keybits)"),
                                         PT_HEXADECIMAL, false, false, _T(""), 58));
    }

    return default_presets;
}

void WPassGen::LoadDefaultSettings()
{
    choicePreset->SetSelection(PT_ALPHANUMERIC);
    choiceType->SetSelection(0);
    checkboxSkipSimilarChar->SetValue(false);
    checkboxSkipSwappedChar->SetValue(false);
    textctrlExtraChar->SetValue(_T(""));
    spinctrlLength->SetValue(12);
    spinctrlNumber->SetValue(10);
    checkboxEnumerate->SetValue(false);

    presetlist = GetDefaultPresets();

    // update dialog logic
    ResetPresetChoice();
    UpdateCheckboxes();
    UpdateKeyStrength();
    GenerateList();

    buttonOK->Disable();
}

void WPassGen::LoadSettings(class wxConfigBase* cfg)
{
    // load settings from wxConfig

    cfg->SetPath(_T("/pwgen"));

    int preset, type, length, number;
    bool skip_similar, skip_swapped, enumerate;
    wxString extrachar;

    cfg->Read(_T("preset"), &preset, PT_ALPHANUMERIC);
    cfg->Read(_T("type"), &type, 0);
    cfg->Read(_T("skip_similar"), &skip_similar, false);
    cfg->Read(_T("skip_swapped"), &skip_swapped, false);
    cfg->Read(_T("extrachar"), &extrachar);
    cfg->Read(_T("length"), &length, 12);
    cfg->Read(_T("number"), &number, 10);
    cfg->Read(_T("enumerate"), &enumerate, false);

    choicePreset->SetSelection(preset);
    choiceType->SetSelection(type);
    checkboxSkipSimilarChar->SetValue(skip_similar);
    checkboxSkipSwappedChar->SetValue(skip_swapped);
    textctrlExtraChar->SetValue(extrachar);
    spinctrlLength->SetValue(length);
    spinctrlNumber->SetValue(number);
    checkboxEnumerate->SetValue(enumerate);

    if (cfg->Exists(_T("/pwgen/presets")))
    {
        presetlist.clear();

        cfg->SetPath(_T("/pwgen/presets"));

        for (unsigned int pi = 0; cfg->HasGroup(wxString::Format(_T("%d"), pi)); ++pi)
        {
            cfg->SetPath(wxString::Format(_T("%d"), pi));

            Preset preset;

            cfg->Read(_T("name"), &preset.name, _("<unknown>"));
            cfg->Read(_T("type"), (int*)&preset.type, 0);
            cfg->Read(_T("skip_similar"), &preset.skip_similar, false);
            cfg->Read(_T("skip_swapped"), &preset.skip_swapped, false);
            cfg->Read(_T("extrachar"), &preset.extrachar);
            cfg->Read(_T("length"), &preset.length, 12);

            presetlist.push_back(preset);

            cfg->SetPath(_T(".."));
        }
    }
    else
    {
        // Load a list of default presets if none are defined in the config
        // storage.

        presetlist = GetDefaultPresets();
    }

    // update dialog logic
    ResetPresetChoice();
    UpdateCheckboxes();
    UpdateKeyStrength();
    GenerateList();

    buttonOK->Disable();
}

void WPassGen::SaveSettings(class wxConfigBase* cfg)
{
    cfg->SetPath(_T("/pwgen"));

    cfg->Write(_T("preset"), choicePreset->GetSelection());
    cfg->Write(_T("type"), choiceType->GetSelection());
    cfg->Write(_T("skip_similar"), checkboxSkipSimilarChar->GetValue());
    cfg->Write(_T("skip_swapped"), checkboxSkipSwappedChar->GetValue());
    cfg->Write(_T("extrachar"), textctrlExtraChar->GetValue());
    cfg->Write(_T("length"), spinctrlLength->GetValue());
    cfg->Write(_T("number"), spinctrlNumber->GetValue());
    cfg->Write(_T("enumerate"), checkboxEnumerate->GetValue());

    cfg->DeleteGroup(_T("/pwgen/presets"));

    if (presetlist != GetDefaultPresets())
    {
        cfg->SetPath(_T("/pwgen/presets"));

        for (unsigned int pi = 0; pi < presetlist.size(); ++pi)
        {
            cfg->SetPath(wxString::Format(_T("%d"), pi));

            cfg->Write(_T("name"), presetlist[pi].name);
            cfg->Write(_T("type"), (int)presetlist[pi].type);
            cfg->Write(_T("skip_similar"), presetlist[pi].skip_similar);
            cfg->Write(_T("skip_swapped"), presetlist[pi].skip_swapped);
            cfg->Write(_T("extrachar"), presetlist[pi].extrachar);
            cfg->Write(_T("length"), presetlist[pi].length);

            cfg->SetPath(_T(".."));
        }
    }

    cfg->Flush();
}

void WPassGen::OnClose(wxCloseEvent& WXUNUSED(event))
{
    if (IsModal())
    {
        EndModal(wxID_CANCEL);
    }
    else
    {
        // Automatically save settings to global config file when closed in
        // non-modal mode.
        wxConfigBase* cfg = wxConfigBase::Get();
        SaveSettings(cfg);

        Destroy();
    }
}

void WPassGen::OnChoicePreset(wxCommandEvent& WXUNUSED(event))
{
    int ni = choicePreset->GetSelection();

    if (ni < 0 || (unsigned int)ni >= presetlist.size()) return;

    const Preset& preset = presetlist[ni];

    choiceType->SetSelection(preset.type);
    checkboxSkipSimilarChar->SetValue(preset.skip_similar);
    checkboxSkipSwappedChar->SetValue(preset.skip_swapped);
    textctrlExtraChar->ChangeValue(preset.extrachar);
    spinctrlLength->SetValue(preset.length);

    UpdateCheckboxes();
    UpdateKeyStrength();
}

void WPassGen::OnButtonPresetAdd(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog dlg(this, _("Enter a descriptive name for the new preset:"), _("Add New Password Preset"));

    if (dlg.ShowModal() != wxID_OK) return;

    Preset preset;

    preset.name = dlg.GetValue();
    preset.type = (pass_type)choiceType->GetSelection();
    preset.skip_similar = checkboxSkipSimilarChar->GetValue();
    preset.skip_swapped = checkboxSkipSwappedChar->GetValue();
    preset.extrachar = textctrlExtraChar->GetValue();
    preset.length = spinctrlLength->GetValue();

    presetlist.push_back(preset);

    ResetPresetChoice();
}

void WPassGen::OnButtonPresetRemove(wxCommandEvent& WXUNUSED(event))
{
    int ni = choicePreset->GetSelection();

    if (ni < 0 || (unsigned int)ni >= presetlist.size()) return;

    presetlist.erase(presetlist.begin() + ni);

    ResetPresetChoice();
}

void WPassGen::OnChoiceType(wxCommandEvent& WXUNUSED(event))
{
    ResetPresetChoice();
    UpdateCheckboxes();
    UpdateKeyStrength();
}

void WPassGen::OnCheckSkipSimilarChar(wxCommandEvent& WXUNUSED(event))
{
    ResetPresetChoice();
    UpdateKeyStrength();
}

void WPassGen::OnCheckSkipSwappedChar(wxCommandEvent& WXUNUSED(event))
{
    ResetPresetChoice();
    UpdateKeyStrength();
}

void WPassGen::OnTextExtraCharChange(wxCommandEvent& WXUNUSED(event))
{
    ResetPresetChoice();
    UpdateKeyStrength();
}

void WPassGen::OnSpinLength(wxSpinEvent& WXUNUSED(event))
{
    ResetPresetChoice();
    UpdateKeyStrength();
}

void WPassGen::OnSpinNumber(wxSpinEvent& event)
{
    pass_type passtype = (pass_type)choiceType->GetSelection();
    bool skip_similar = checkboxSkipSimilarChar->GetValue();
    bool skip_swapped = checkboxSkipSwappedChar->GetValue();
    wxString extrachar = textctrlExtraChar->GetValue();
    int passlen = spinctrlLength->GetValue();
    bool enumerate = checkboxEnumerate->GetValue();
    int passnumber = event.GetPosition();

    Preset preset(_T(""), passtype, skip_similar, skip_swapped, extrachar, passlen);

    if (standalone)
    {
        // Fill wxTextCtrl with passwords

        wxString oldtext = textctrlPasslist->GetValue();
        wxStringTokenizer oldtexttoken(oldtext, _T("\n"));

        wxString newtext;

        for (int i = 0; i < passnumber; i++)
        {
            if (oldtexttoken.HasMoreTokens())
            {
                newtext += oldtexttoken.GetNextToken() + _T("\n");
            }
            else
            {
                if (enumerate)
                    newtext += wxString::Format(_T("%u "), i);

                newtext += MakePassword(preset) + _T("\n");
            }
        }

        textctrlPasslist->SetValue(newtext);
    }
    else
    {
        // Fill wxListCtrl with passwords

        int oldcount = listctrlPasslist->GetItemCount();

        for (int i = oldcount; i > passnumber; --i)
        {
            listctrlPasslist->DeleteItem(i - 1);
        }

        for (int i = oldcount; i < passnumber; i++)
        {
            listctrlPasslist->InsertItem(i, MakePassword(preset));
        }
    }

    buttonOK->Disable();
}

void WPassGen::OnCheckEnumerate(wxCommandEvent& WXUNUSED(event))
{ }

void WPassGen::OnButtonGenerate(wxCommandEvent& WXUNUSED(event))
{
    GenerateList();
}

void WPassGen::OnPasslistSelected(wxListEvent& WXUNUSED(event))
{
    buttonOK->Enable(listctrlPasslist->GetSelectedItemCount() != 0);
}

void WPassGen::OnPasslistActivated(wxListEvent& event)
{
    OnButtonOK(event);
}

void WPassGen::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
    if (!IsModal()) return;

    EndModal(wxID_OK);
}

void WPassGen::OnButtonCancel(wxCommandEvent& WXUNUSED(event))
{
    if (!IsModal()) return;

    EndModal(wxID_CANCEL);
}

void WPassGen::OnButtonClose(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void WPassGen::OnButtonAbout(wxCommandEvent& WXUNUSED(event))
{
    PGWAbout dlg(this);
    dlg.ShowModal();
}

// wxGlade: add WPassGen event handlers

void WPassGen::ResetPresetChoice()
{
    choicePreset->Clear();

    for (presetlist_type::const_iterator pi = presetlist.begin();
         pi != presetlist.end(); ++pi)
    {
        int ni = choicePreset->Append(pi->name);

        int passtype = choiceType->GetSelection();
        bool skip_similar = checkboxSkipSimilarChar->GetValue();
        bool skip_swapped = checkboxSkipSwappedChar->GetValue();
        wxString extrachar = textctrlExtraChar->GetValue();
        int length = spinctrlLength->GetValue();

        if (pi->type == passtype &&
            pi->skip_similar == skip_similar &&
            pi->skip_swapped == skip_swapped &&
            pi->extrachar == extrachar &&
            pi->length == length)
        {
            choicePreset->SetSelection(ni);
        }
    }
}

void WPassGen::UpdateCheckboxes()
{
    checkboxSkipSimilarChar->Enable(IsAllowedSimilar());
    checkboxSkipSwappedChar->Enable(IsAllowedSwapped());
    textctrlExtraChar->Enable(IsAllowedExtraChar());
    spinctrlLength->Enable(IsAllowedLength());
}

void WPassGen::UpdateKeyStrength()
{
    pass_type passtype = (pass_type)choiceType->GetSelection();
    bool skip_similar = checkboxSkipSimilarChar->GetValue();
    bool skip_swapped = checkboxSkipSwappedChar->GetValue();
    wxString extrachar = textctrlExtraChar->GetValue();
    int passlen = spinctrlLength->GetValue();

    Preset preset(_T(""), passtype, skip_similar, skip_swapped, extrachar, passlen);

    double keybits = GetTypeKeybits(preset);

    if (IsAllowedLength()) keybits *= passlen;

    wxString ss = wxString::Format(_("%.1f keybits"), keybits);

    textctrlStrength->SetValue(ss);
}

void WPassGen::GenerateList()
{
    pass_type passtype = (pass_type)choiceType->GetSelection();
    bool skip_similar = checkboxSkipSimilarChar->GetValue();
    bool skip_swapped = checkboxSkipSwappedChar->GetValue();
    wxString extrachar = textctrlExtraChar->GetValue();
    int passlen = spinctrlLength->GetValue();
    bool enumerate = checkboxEnumerate->GetValue();
    int passnumber = spinctrlNumber->GetValue();

    Preset preset(_T(""), passtype, skip_similar, skip_swapped, extrachar, passlen);

    if (standalone)
    {
        // Fill wxTextCtrl with passwords

        wxString text;

        for (int i = 0; i < passnumber; i++)
        {
            if (enumerate)
                text += wxString::Format(_T("%u "), i);

            text += MakePassword(preset) + _T("\n");
        }

        textctrlPasslist->SetValue(text);
    }
    else
    {
        // Fill wxListCtrl with passwords

        listctrlPasslist->DeleteAllItems();

        for (int i = 0; i < passnumber; i++)
        {
            listctrlPasslist->InsertItem(i, MakePassword(preset));
        }
    }

    buttonOK->Disable();
}

wxArrayString WPassGen::GetSelectedPassword() const
{
    wxArrayString arrstr;

    long item = -1;
    while ((item = listctrlPasslist->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) >= 0)
    {
        arrstr.Add(listctrlPasslist->GetItemText(item));
    }

    return arrstr;
}

bool WPassGen::IsAllowedSimilar() const
{
    pass_type passtype = (pass_type)choiceType->GetSelection();

    switch (passtype)
    {
    case PT_PRONOUNCEABLE:
        return false;

    case PT_ALPHANUMERICSYMBOL:
    case PT_ALPHANUMERIC:
        return true;

    case PT_ALPHA:
    case PT_ALPHALOWER:
    case PT_ALPHAUPPER:
        return false;

    case PT_HEXADECIMAL:
    case PT_NUMERIC:
    case PT_PORTNUMBER:
        return false;
    }

    return false;
}

bool WPassGen::IsAllowedSwapped() const
{
    pass_type passtype = (pass_type)choiceType->GetSelection();

    switch (passtype)
    {
    case PT_PRONOUNCEABLE:
        return false;

    case PT_ALPHANUMERICSYMBOL:
    case PT_ALPHANUMERIC:
        return true;

    case PT_ALPHA:
    case PT_ALPHALOWER:
    case PT_ALPHAUPPER:
        return true;

    case PT_HEXADECIMAL:
    case PT_NUMERIC:
    case PT_PORTNUMBER:
        return false;
    }

    return false;
}

bool WPassGen::IsAllowedExtraChar() const
{
    pass_type passtype = (pass_type)choiceType->GetSelection();

    switch (passtype)
    {
    case PT_PRONOUNCEABLE:
        return false;

    case PT_ALPHANUMERICSYMBOL:
    case PT_ALPHANUMERIC:
    case PT_ALPHA:
    case PT_ALPHALOWER:
    case PT_ALPHAUPPER:
    case PT_NUMERIC:
        return true;

    case PT_HEXADECIMAL:
    case PT_PORTNUMBER:
        return false;
    }

    return false;
}

bool WPassGen::IsAllowedLength() const
{
    pass_type passtype = (pass_type)choiceType->GetSelection();

    switch (passtype)
    {
    case PT_PRONOUNCEABLE:
    case PT_ALPHANUMERICSYMBOL:
    case PT_ALPHANUMERIC:
    case PT_ALPHA:
    case PT_ALPHALOWER:
    case PT_ALPHAUPPER:
    case PT_HEXADECIMAL:
    case PT_NUMERIC:
        return true;

    case PT_PORTNUMBER:
        return false;
    }

    return false;
}

// *** Static Password Generator Functions ***

const wxString WPassGen::GetTypeName(pass_type pt)
{
    switch (pt)
    {
    case PT_ALPHANUMERICSYMBOL:
        return _("Random Letters + Numbers + Symbols");

    case PT_ALPHANUMERIC:
        return _("Random Letters + Numbers");

    case PT_ALPHA:
        return _("Random Letters");

    case PT_ALPHALOWER:
        return _("Random Lower-case Letters");

    case PT_PRONOUNCEABLE:
        return _("Pronounceable Lower-case Letters");

    case PT_ALPHAUPPER:
        return _("Random Upper-case Letters");

    case PT_HEXADECIMAL:
        return _("Random Hexadecimal Number");

    case PT_NUMERIC:
        return _("Random Numbers");

    case PT_PORTNUMBER:
        return _("Random IP Port Number");

    default:
        return _("Unknown");
    }
}

const wxChar* WPassGen::GetType0Letters(pass_type pt, bool skip_similar, bool skip_swapped)
{
    switch (pt)
    {
    case PT_PRONOUNCEABLE:
        assert(0);
        return _T("");

    case PT_ALPHANUMERICSYMBOL:

        if (skip_similar && skip_swapped)
            return _T("ABCDEFGHIJKLMNPQRSTUVWXabcdefghijkmnopqrstuvwx23456789!?\"#$%&*()[]{}<>,.;:+-=/_");
        else if (skip_similar && !skip_swapped)
            return _T("ABCDEFGHIJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789!?\"#$%&*()[]{}<>,.;:+-=/_");
        else if (!skip_similar && skip_swapped)
            return _T("ABCDEFGHIJKLMNOPQRSTUVWXabcdefghijklmnopqrstuvwx0123456789!?\"#$%&*()[]{}<>,.;:+-=/_");
        else
            return _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!?\"#$%&*()[]{}<>,.;:+-=/_");

    case PT_ALPHANUMERIC:

        if (skip_similar && skip_swapped)
            return _T("ABCDEFGHIJKLMNPQRSTUVWXabcdefghijkmnopqrstuvwx23456789");
        else if (skip_similar && !skip_swapped)
            return _T("ABCDEFGHIJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz23456789");
        else if (!skip_similar && skip_swapped)
            return _T("ABCDEFGHIJKLMNOPQRSTUVWXabcdefghijklmnopqrstuvwx0123456789");
        else
            return _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    case PT_ALPHA:

        if (skip_swapped)
            return _T("ABCDEFGHIJKLMNOPQRSTUVWXabcdefghijklmnopqrstuvwx");
        else
            return _T("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    case PT_ALPHALOWER:

        if (skip_swapped)
            return _T("abcdefghijklmnopqrstuvwx");
        else
            return _T("abcdefghijklmnopqrstuvwxyz");

    case PT_ALPHAUPPER:

        if (skip_swapped)
            return _T("ABCDEFGHIJKLMNOPQRSTUVWX");
        else
            return _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    case PT_HEXADECIMAL:

        return _T("0123456789ABCDEF");

    case PT_NUMERIC:

        return _T("0123456789");

    case PT_PORTNUMBER:
        assert(0);
        return _T("");
    }

    return _T("");
}

wxString WPassGen::GetType0LettersExtra(const Preset& preset)
{
    switch (preset.type)
    {
    case PT_PRONOUNCEABLE:
        assert(0);
        return _T("");

    case PT_ALPHANUMERICSYMBOL:
    case PT_ALPHANUMERIC:
    case PT_ALPHA:
    case PT_ALPHALOWER:
    case PT_ALPHAUPPER:
    case PT_NUMERIC:

        return wxString(GetType0Letters(preset.type, preset.skip_similar, preset.skip_swapped)) + preset.extrachar;

    case PT_HEXADECIMAL:

        return GetType0Letters(preset.type, preset.skip_similar, preset.skip_swapped);

    case PT_PORTNUMBER:
        assert(0);
        return _T("");
    }

    return _T("");
}

float WPassGen::GetTypeKeybits(const Preset& preset)
{
    switch (preset.type)
    {
    case PT_PRONOUNCEABLE:
        // The FIPS-181 standard says "Approximately 18 million 6-character,
        // 5.7 billion 8-character, and 1.6 trillion 10-character passwords can
        // be created by the program." This is equivalent to about 4.05 keybits
        // per letter.
        return 4.05;

    case PT_ALPHANUMERICSYMBOL:
    case PT_ALPHANUMERIC:
    case PT_ALPHA:
    case PT_ALPHALOWER:
    case PT_ALPHAUPPER:
    case PT_HEXADECIMAL:
    case PT_NUMERIC:
    {
        wxString letters = GetType0LettersExtra(preset);

        // remove duplicate letters
        for (unsigned int i = 0; i < letters.size(); ++i)
        {
            for (unsigned int j = i + 1; j < letters.size(); ++j)
            {
                if (letters[i] == letters[j]) letters.Remove(j--, 1);
            }
        }

        return log(letters.size()) / log(2);
    }

    case PT_PORTNUMBER:
        return log(65536) / log(2);
    }

    return 0;
}

wxString WPassGen::MakePasswordType0(unsigned int len, const wxString& letters)
{
    wxString s;
    unsigned int lettlen = letters.size();

    for (unsigned int i = 0; i < len; ++i)
    {
        s.Append(letters[Enctain::RNG::random_uint() % lettlen]);
    }

    return s;
}

wxString WPassGen::MakePassword(const Preset& preset)
{
    switch (preset.type)
    {
    case PT_PRONOUNCEABLE:
    {
        FIPS181 fips181(Enctain::RNG::random_uint);

        std::string word, hyphenated_word;
        fips181.randomword(word, hyphenated_word, preset.length, preset.length);

        return wxString(word.data(), wxConvUTF8, word.size());
    }

    case PT_ALPHANUMERICSYMBOL:
    case PT_ALPHANUMERIC:
    case PT_ALPHA:
    case PT_ALPHALOWER:
    case PT_ALPHAUPPER:
    case PT_HEXADECIMAL:
    case PT_NUMERIC:
    {
        wxString letters = GetType0LettersExtra(preset);

        return MakePasswordType0(preset.length, letters);
    }

    case PT_PORTNUMBER:
    {
        return wxString::Format(_T("%u"), Enctain::RNG::random_uint() % 65536);
    }
    }

    return _T("");
}

// *** PGWAbout - Password Generator About Dialog ***

PGWAbout::PGWAbout(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long WXUNUSED(style))
    : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
    // begin wxGlade: PGWAbout::PGWAbout
    bitmapIcon = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap);
    bitmapWeb = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap);
    hyperlink1 = new wxHyperlinkCtrl(this, wxID_ANY, _("Visit http://panthema.net/2009/cryptote/"), _("http://panthema.net/2009/cryptote/"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxHL_CONTEXTMENU | wxHL_ALIGN_LEFT);
    buttonOK = new wxButton(this, wxID_OK, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

    #include "art/pwgen-48.h"
    bitmapIcon->SetBitmap(wxBitmapFromMemory(pwgen_48_png));

    #include "art/web-16.h"
    bitmapWeb->SetBitmap(wxBitmapFromMemory(web_16_png));

    Layout();
    GetSizer()->Fit(this);
    Centre();
}

void PGWAbout::set_properties()
{
    // begin wxGlade: PGWAbout::set_properties
    SetTitle(_("About CryptoTE Password Generator"));
    // end wxGlade
}

void PGWAbout::do_layout()
{
    // begin wxGlade: PGWAbout::do_layout
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer4 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer5 = new wxBoxSizer(wxVERTICAL);
    sizer2->Add(bitmapIcon, 0, wxALL, 8);
    wxStaticText* label1 = new wxStaticText(this, wxID_ANY, _("CryptoTE\nPassword Generator"));
    label1->SetFont(wxFont(18, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
    sizer_3->Add(label1, 0, wxALL, 6);
    wxStaticText* label2 = new wxStaticText(this, wxID_ANY, _("This is a stand-alone program containing only the\nrandom password generator found in CryptoTE\ntext editor component."));
    sizer_3->Add(label2, 0, wxALL, 6);
    wxStaticText* label3 = new wxStaticText(this, wxID_ANY, _("The random password generator can generate\npronounceable English passwords or general upper-\nand/or lowercase letter and digit combinations.\nIt also displays the theoretic strength in key bits of\nthe chosen generator settings."));
    sizer_3->Add(label3, 0, wxALL, 6);
    wxStaticText* label4 = new wxStaticText(this, wxID_ANY, _("Copyright 2009 Timo Bingmann\nReleased under the GNU General Public License v2"));
    sizer_3->Add(label4, 0, wxALL, 6);
    sizer4->Add(bitmapWeb, 0, wxALL | wxALIGN_CENTER_VERTICAL, 6);
    sizer5->Add(hyperlink1, 1, wxEXPAND, 0);
    wxStaticText* label5 = new wxStaticText(this, wxID_ANY, _("for updates and more."));
    sizer5->Add(label5, 0, wxALL, 0);
    sizer4->Add(sizer5, 1, wxEXPAND, 0);
    sizer_3->Add(sizer4, 0, wxALL | wxEXPAND, 6);
    sizer2->Add(sizer_3, 1, wxEXPAND, 0);
    sizer1->Add(sizer2, 1, wxALL | wxEXPAND, 6);
    sizer1->Add(buttonOK, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 6);
    SetSizer(sizer1);
    sizer1->Fit(this);
    Layout();
    // end wxGlade

    label1->SetLabel(wxString::Format(_("CryptoTE\nPassword Generator %s"), _(VERSION)));
}

/******************************************************************************/
