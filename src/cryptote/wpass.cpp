/*******************************************************************************
 * src/cryptote/wpass.cpp
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

#include "wpass.h"
#include "wcryptote.h"
#include "bmpcat.h"
#include "wmsgdlg.h"
#include "common/tools.h"

#include <wx/filename.h>
#include <wx/imaglist.h>

// begin wxGlade: ::extracode
// end wxGlade

WPasswordList::WPasswordList(WCryptoTE* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long WXUNUSED(style))
    : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxTHICK_FRAME),
      wmain(parent)
{
    SetMinSize(wxSize(400, 350));

    // begin wxGlade: WPasswordList::WPasswordList
    listbox = new wxSimpleImageListBox(this, myID_PASSLIST);
    buttonAdd = new wxButton(this, wxID_ADD, wxEmptyString);
    buttonChange = new wxButton(this, myID_CHANGE, _("&Change"));
    buttonRemove = new wxButton(this, myID_REMOVE, _("&Remove"));
    buttonOK = new wxButton(this, wxID_OK, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

    ReinsertList();
}

void WPasswordList::set_properties()
{
    // begin wxGlade: WPasswordList::set_properties
    SetTitle(_("Password Slots - CryptoTE"));
    // end wxGlade
}

void WPasswordList::do_layout()
{
    // begin wxGlade: WPasswordList::do_layout
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* sizer3 = new wxGridSizer(1, 3, 0, 0);
    sizer2->Add(listbox, 1, wxLEFT | wxRIGHT | wxTOP | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 12);
    sizer3->Add(buttonAdd, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(buttonChange, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(buttonRemove, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 6);
    sizer2->Add(sizer3, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    sizer1->Add(sizer2, 1, wxEXPAND, 0);
    wxStaticLine* staticline1 = new wxStaticLine(this, wxID_ANY);
    sizer1->Add(staticline1, 0, wxEXPAND, 0);
    sizer1->Add(buttonOK, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 6);
    SetSizer(sizer1);
    sizer1->Fit(this);
    Layout();
    Centre();
    // end wxGlade
}

BEGIN_EVENT_TABLE(WPasswordList, wxDialog)
    // begin wxGlade: WPasswordList::event_table
    EVT_BUTTON(wxID_ADD, WPasswordList::OnButtonAdd)
    EVT_BUTTON(myID_CHANGE, WPasswordList::OnButtonChange)
    EVT_BUTTON(myID_REMOVE, WPasswordList::OnButtonRemove)
    EVT_BUTTON(wxID_OK, WPasswordList::OnButtonOK)
    // end wxGlade
    EVT_LISTBOX(wxID_ANY, WPasswordList::OnListboxSelectionChanged)
END_EVENT_TABLE();

void WPasswordList::ReinsertList()
{
    listbox->Clear();
    listbox->SetSelection(-1);
    listbox->SetTextSpacing(7);

    BitmapCatalog* bitmapcatalog = BitmapCatalog::GetSingleton();
    wxBitmap bmp_userkeyslot = bitmapcatalog->GetBitmap(myID_IMAGE_USERKEYSLOT);
    wxBitmap bmp_userkeyslot_active = bitmapcatalog->GetBitmap(myID_IMAGE_USERKEYSLOT_ACTIVE);

    Enctain::Container cnt = wmain->container;

    unsigned int slots = cnt.CountKeySlots();

    for (unsigned int s = 0; s < slots; ++s)
    {
        wxString text = wxString::Format(_("Slot: %u - "), s);

        std::string desc = cnt.GetGlobalEncryptedProperty("KeySlot-" + toSTLString(s) + "-Description");
        if (desc.empty()) text += _("<empty-description>");
        else text += strSTL2WX(desc);

        std::string ctime = cnt.GetGlobalEncryptedProperty("KeySlot-" + toSTLString(s) + "-CTime");

        text += _T("\n    ");
        text += _("Created:");
        text += _T(" ");

        if (ctime.size() == sizeof(time_t)) {
            wxDateTime datetime(*(time_t*)ctime.data());
            text += datetime.Format(_("%c"));
        }
        else {
            text += _("<never-created>");
        }

        std::string atime = cnt.GetGlobalEncryptedProperty("KeySlot-" + toSTLString(s) + "-ATime");

        text += _T("\n    ");
        text += _("Last Match:");
        text += _T(" ");

        if (atime.size() == sizeof(time_t)) {
            wxDateTime datetime(*(time_t*)atime.data());
            text += datetime.Format(_("%c"));
        }
        else {
            text += _("<never-matched>");
        }

        if (cnt.GetUsedKeySlot() == (int)s) {
            text += _T("\n    ");
            text += _("Active");
            text += _T(" ");
        }

        listbox->Append(text);

        if (cnt.GetUsedKeySlot() == (int)s) {
            listbox->SetBitmap(s, bmp_userkeyslot_active);
        }
        else {
            listbox->SetBitmap(s, bmp_userkeyslot);
        }
    }

    buttonChange->Enable(false);
    buttonRemove->Enable(false);
}

void WPasswordList::OnListboxSelectionChanged(wxCommandEvent& WXUNUSED(event))
{
    int slot = listbox->GetSelection();

    buttonChange->Enable(slot >= 0);
    buttonRemove->Enable(slot >= 0);
}

void WPasswordList::OnButtonAdd(wxCommandEvent& WXUNUSED(event))
{
    Enctain::Container cnt = wmain->container;

    WSetPassword passdlg(wmain, this, wmain->GetSavedFilename());
    if (passdlg.ShowModal() != wxID_OK) return;

    unsigned int newslot = cnt.AddKeySlot(strWX2STL(passdlg.GetPass()));

    // Add key slot metadata.
    cnt.SetGlobalEncryptedProperty("KeySlot-" + toSTLString(newslot) + "-CTime", strTimeStampNow());
    cnt.DeleteGlobalEncryptedProperty("KeySlot-" + toSTLString(newslot) + "-ATime");
    cnt.SetGlobalEncryptedProperty("KeySlot-" + toSTLString(newslot) + "-Description", strWX2STL(passdlg.GetDescription()));

    wmain->SetModified();

    ReinsertList();
}

void WPasswordList::OnButtonChange(wxCommandEvent& WXUNUSED(event))
{
    Enctain::Container cnt = wmain->container;

    int slot = listbox->GetSelection();
    if (slot < 0) return;
    if (slot >= (int)cnt.CountKeySlots()) return;

    WSetPassword passdlg(wmain, this, wmain->GetSavedFilename());
    passdlg.SetDescription(strSTL2WX(cnt.GetGlobalEncryptedProperty("KeySlot-" + toSTLString(slot) + "-Description")));

    if (passdlg.ShowModal() != wxID_OK) return;

    cnt.ChangeKeySlot(slot, strWX2STL(passdlg.GetPass()));

    cnt.SetGlobalEncryptedProperty("KeySlot-" + toSTLString(slot) + "-CTime", strTimeStampNow());
    cnt.DeleteGlobalEncryptedProperty("KeySlot-" + toSTLString(slot) + "-ATime");
    cnt.SetGlobalEncryptedProperty("KeySlot-" + toSTLString(slot) + "-Description", strWX2STL(passdlg.GetDescription()));

    wmain->SetModified();

    ReinsertList();
}

void WPasswordList::OnButtonRemove(wxCommandEvent& WXUNUSED(event))
{
    Enctain::Container cnt = wmain->container;

    int slot = listbox->GetSelection();
    if (slot < 0) return;
    if (slot >= (int)cnt.CountKeySlots()) return;

    cnt.DeleteKeySlot(slot);

    for (unsigned int s = slot; s < cnt.CountKeySlots(); ++s)
    {
        std::string copy = cnt.GetGlobalEncryptedProperty("KeySlot-" + toSTLString(s + 1) + "-Description");
        cnt.SetGlobalEncryptedProperty("KeySlot-" + toSTLString(s) + "-Description", copy);

        copy = cnt.GetGlobalEncryptedProperty("KeySlot-" + toSTLString(s + 1) + "-ATime");
        cnt.SetGlobalEncryptedProperty("KeySlot-" + toSTLString(s) + "-ATime", copy);

        copy = cnt.GetGlobalEncryptedProperty("KeySlot-" + toSTLString(s + 1) + "-CTime");
        cnt.SetGlobalEncryptedProperty("KeySlot-" + toSTLString(s) + "-CTime", copy);
    }

    cnt.DeleteGlobalEncryptedProperty("KeySlot-" + toSTLString(cnt.CountKeySlots()) + "-ATime");
    cnt.DeleteGlobalEncryptedProperty("KeySlot-" + toSTLString(cnt.CountKeySlots()) + "-CTime");
    cnt.DeleteGlobalEncryptedProperty("KeySlot-" + toSTLString(cnt.CountKeySlots()) + "-Description");

    wmain->SetModified();

    ReinsertList();
}

void WPasswordList::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_OK);
}

WSetPassword::WSetPassword(class WCryptoTE* _wmain, class wxWindow* parent, const wxString& filename, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long WXUNUSED(style))
    : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxTHICK_FRAME),
      wmain(_wmain)
{
    // begin wxGlade: WSetPassword::WSetPassword
    labelQuery = new wxStaticText(this, wxID_ANY, _("Set Password for \"abc.ect\":"));
    textctrlPass = new wxTextCtrl(this, myID_TEXTPASS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_PASSWORD);
    gaugeStrength = new wxGauge(this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL | wxGA_SMOOTH);
    textctrlVerify = new wxTextCtrl(this, myID_TEXTVERIFY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_PASSWORD);
    textctrlDescription = new wxTextCtrl(this, myID_TEXTDESCRIPTION, wxEmptyString);
    hyperlinkAdvice = new wxHyperlinkCtrl(this, myID_HYPERLINK_ADVICE, _("Advice on Choosing a Password"), _T(""));
    buttonOK = new wxButton(this, wxID_OK, wxEmptyString);
    buttonCancel = new wxButton(this, wxID_CANCEL, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

    Layout();

    {
        wxSize maxsize = sizerGauge->GetSize();

        if (maxsize.GetWidth() < sizerVerify->GetSize().GetWidth())
            maxsize.SetWidth(sizerVerify->GetSize().GetWidth());
        if (maxsize.GetHeight() < sizerVerify->GetSize().GetHeight())
            maxsize.SetHeight(sizerVerify->GetSize().GetHeight());

        sizerGauge->SetMinSize(maxsize);
        sizerVerify->SetMinSize(maxsize);
    }

    GetSizer()->Hide(sizerVerify, true);
    Layout();
    GetSizer()->Fit(this);

    state = 0;

    wxFileName fn(filename);
    labelQuery->SetLabel(wxString::Format(_("Set Password for \"%s\":"), fn.GetFullName().c_str()));

    textctrlDescription->SetValue(_("User ") + wxGetUserId());

    buttonOK->Disable();
    textctrlPass->SetFocus();
}

void WSetPassword::set_properties()
{
    // begin wxGlade: WSetPassword::set_properties
    SetTitle(_("Set Password - CryptoTE"));
    // end wxGlade
}

void WSetPassword::do_layout()
{
    // begin wxGlade: WSetPassword::do_layout
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* sizer7 = new wxGridSizer(1, 2, 0, 0);
    wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer4 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer3_ = new wxBoxSizer(wxHORIZONTAL);
    sizer2->Add(labelQuery, 0, wxALL, 8);
    sizer2->Add(textctrlPass, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 8);
    sizer3_->Add(20, 20, 1, 0, 0);
    wxStaticText* label1 = new wxStaticText(this, wxID_ANY, _("Estimated Strength:"));
    sizer3_->Add(label1, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 8);
    sizer3_->Add(gaugeStrength, 2, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND | wxALIGN_CENTER_VERTICAL, 8);
    sizer3->Add(sizer3_, 0, wxEXPAND, 0);
    sizer3->Add(0, 0, 1, 0, 0);
    sizer2->Add(sizer3, 1, wxEXPAND, 0);
    wxStaticText* labelVerify = new wxStaticText(this, wxID_ANY, _("Enter password again:"));
    sizer4->Add(labelVerify, 0, wxALL, 8);
    sizer4->Add(textctrlVerify, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 8);
    sizer2->Add(sizer4, 1, wxEXPAND, 0);
    wxStaticText* labelDescription = new wxStaticText(this, wxID_ANY, _("Description:"));
    sizer2->Add(labelDescription, 0, wxALL, 8);
    sizer2->Add(textctrlDescription, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 8);
    sizer5->Add(1, 1, 1, 0, 0);
    wxStaticText* labelConsider = new wxStaticText(this, wxID_ANY, _("Consider reading: "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    sizer5->Add(labelConsider, 0, wxRIGHT, 2);
    sizer5->Add(hyperlinkAdvice, 0, 0, 0);
    sizer5->Add(1, 1, 1, 0, 0);
    sizer2->Add(sizer5, 0, wxALL | wxEXPAND, 8);
    sizer1->Add(sizer2, 1, wxALL | wxEXPAND, 8);
    wxStaticLine* staticline1 = new wxStaticLine(this, wxID_ANY);
    sizer1->Add(staticline1, 0, wxEXPAND, 0);
    sizer7->Add(buttonOK, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer7->Add(buttonCancel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 6);
    sizer1->Add(sizer7, 0, wxEXPAND, 0);
    SetSizer(sizer1);
    sizer1->Fit(this);
    Layout();
    Centre();
    // end wxGlade

    sizerGauge = sizer3;
    sizerVerify = sizer4;
}

BEGIN_EVENT_TABLE(WSetPassword, wxDialog)
    // begin wxGlade: WSetPassword::event_table
    EVT_TEXT_ENTER(myID_TEXTPASS, WSetPassword::OnTextPassEnter)
    EVT_TEXT(myID_TEXTPASS, WSetPassword::OnTextPass)
    EVT_TEXT_ENTER(myID_TEXTVERIFY, WSetPassword::OnTextVerifyEnter)
    EVT_BUTTON(wxID_OK, WSetPassword::OnButtonOK)
    // end wxGlade
    EVT_HYPERLINK(myID_HYPERLINK_ADVICE, WSetPassword::OnHyperlinkAdvice)
END_EVENT_TABLE();

// *** Password Strength Estimation ***
// Idea based on some Internet web page to which I don't have the URL
// anymore. Implemented in code by myself.

static inline std::string checkRepetition(unsigned int plen, const std::string& str)
{
    std::string out = "";

    for (unsigned int i = 0; i < str.size(); ++i)
    {
        bool repeated = true;
        unsigned int j;

        for (j = 0; j < plen && (i + j + plen) < str.size(); ++j)
        {
            repeated = (repeated && (str[i + j] == str[i + j + plen]));
        }

        if (j < plen) repeated = false;

        if (repeated) {
            i += plen - 1;
            repeated = false;
        }
        else {
            out += str[i];
        }
    }
    return out;
}

static inline bool my_isspecial(char c)
{
    switch (c)
    {
    case '!': case '@': case '#': case '$': case '%':
    case '^': case '&': case '*': case '(': case ')':
    case '~': case '-': case '_': case '=': case '+':

    case '[': case ']': case '{': case '}':
    case ';': case ':': case '\'': case '"':
    case '|': case '\\':
    case ',': case '.': case '<': case '>':
    case '?': case '/':

        return true;

    default:
        return false;
    }
}

static int calcPasswordStrength(const std::string& str)
{
    // if password has less than 4 characters it is way too short.
    if (str.size() < 4) return 0;

    // base size: 4 bits per character
    unsigned int score = str.size() * 4;

    // for each repeated character reduce score

    score += (checkRepetition(1, str).size() - str.size()) * 1;
    score += (checkRepetition(2, str).size() - str.size()) * 1;
    score += (checkRepetition(3, str).size() - str.size()) * 1;
    score += (checkRepetition(4, str).size() - str.size()) * 1;

    // classify characters in password

    unsigned int numbers = 0;
    unsigned int specialchars = 0;
    unsigned int upperchars = 0;
    unsigned int lowerchars = 0;

    for (std::string::const_iterator si = str.begin();
         si != str.end(); ++si)
    {
        if (isdigit(*si)) numbers++;
        if (islower(*si)) lowerchars++;
        if (isupper(*si)) upperchars++;
        if (my_isspecial(*si)) specialchars++;
    }

    // +5 for passwords with >= 3 numbers
    if (numbers >= 3) score += 5;

    // +5 for passwords with >= 2 special characters
    if (specialchars >= 2) score += 5;

    // +10 for passwords with both lower and upper chars.
    if (upperchars > 0 && lowerchars > 0) score += 10;

    // +10 for passwords with both chars and numbers
    if (numbers > 0 && (upperchars + lowerchars) > 0) score += 10;

    // +15 for passwords with both chars and specials
    if (specialchars > 0 && (upperchars + lowerchars) > 0) score += 15;

    // +15 for passwords with both numbers and specials
    if (numbers > 0 && (upperchars + lowerchars) > 0) score += 15;

    // -10 for password with only characters
    if (upperchars + lowerchars == str.size()) score -= 10;

    // -15 for password with only numbers
    if (numbers == str.size()) score -= 15;

    return score;
}

void WSetPassword::OnTextPass(wxCommandEvent& WXUNUSED(event))
{
    std::string str = strWX2STL(textctrlPass->GetValue());

    unsigned int score = calcPasswordStrength(str);

    gaugeStrength->SetValue(score > 100 ? 100 : score);

    wxColour gaugecolor;

    if (score < 34) {
        gaugecolor = wxColour(230, 0, 0);
        if (score < 10)
            buttonOK->Disable();
        else
            buttonOK->Enable();
    }
    else if (score < 67) {
        gaugecolor = wxColour(230, 230, 0);
        buttonOK->Enable();
    }
    else {
        gaugecolor = wxColour(0, 230, 0);
        buttonOK->Enable();
    }

    // Different looks on Windows and GTK
#ifdef __WXGTK__
    gaugeStrength->SetBackgroundColour(gaugecolor);
#else
    gaugeStrength->SetForegroundColour(gaugecolor);
#endif
}

void WSetPassword::OnTextPassEnter(wxCommandEvent& event)
{
    if (buttonOK->IsEnabled())
        OnButtonOK(event);
}

const wxString& WSetPassword::GetPass() const
{
    return pass;
}

const wxString& WSetPassword::GetDescription() const
{
    return description;
}

void WSetPassword::SetDescription(const wxString& desc)
{
    textctrlDescription->SetValue(desc);
}

void WSetPassword::OnTextVerifyEnter(wxCommandEvent& event)
{
    if (buttonOK->IsEnabled())
        OnButtonOK(event);
}

void WSetPassword::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
    if (state == 0)
    {
        // Swap display panes
        GetSizer()->Hide(sizerGauge, true);
        GetSizer()->Show(sizerVerify, true, true);
        Layout();

        pass = textctrlPass->GetValue();
        textctrlPass->ChangeValue(_T(""));
        textctrlPass->Disable();
        buttonOK->Enable();

        // Show legal notice dialog in between entries
        WLegalNotice dlglegal(this);
        dlglegal.ShowModal();

        textctrlVerify->SetFocus();

        state = 1;
    }
    else
    {
        if (textctrlVerify->GetValue() != pass)
        {
            wxMessageDialogErrorOK(this, _("Entered passwords no not match."));
        }
        else
        {
            description = textctrlDescription->GetValue();
            EndModal(wxID_OK);
        }
    }
}

void WSetPassword::OnHyperlinkAdvice(wxHyperlinkEvent& WXUNUSED(event))
{
    wmain->GetHtmlHelpController()->Display(_("cryptote_aboutencryption.html#AdvicePassword"));
}

// wxGlade: add WSetPassword event handlers

WGetPassword::WGetPassword(wxWindow* parent, const wxString& filename, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long WXUNUSED(style))
    : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxTHICK_FRAME)
{
    // begin wxGlade: WGetPassword::WGetPassword
    labelQuery = new wxStaticText(this, wxID_ANY, _("Enter Password for abc.ect:"));
    textctrlPass = new wxTextCtrl(this, myID_TEXTPASS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER | wxTE_PASSWORD);
    buttonOK = new wxButton(this, wxID_OK, wxEmptyString);
    buttonCancel = new wxButton(this, wxID_CANCEL, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

    wxFileName fn(filename);
    labelQuery->SetLabel(wxString::Format(_("Enter Password for \"%s\":"), fn.GetFullName().c_str()));

    textctrlPass->SetFocus();
}

void WGetPassword::set_properties()
{
    // begin wxGlade: WGetPassword::set_properties
    SetTitle(_("Enter Password - CryptoTE"));
    // end wxGlade
}

void WGetPassword::do_layout()
{
    // begin wxGlade: WGetPassword::do_layout
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* sizer3 = new wxGridSizer(1, 2, 0, 0);
    wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);
    sizer2->Add(labelQuery, 0, wxALL, 8);
    sizer2->Add(textctrlPass, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND, 8);
    sizer1->Add(sizer2, 1, wxALL | wxEXPAND, 8);
    wxStaticLine* staticline1 = new wxStaticLine(this, wxID_ANY);
    sizer1->Add(staticline1, 0, wxEXPAND, 0);
    sizer3->Add(buttonOK, 0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(buttonCancel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 6);
    sizer1->Add(sizer3, 0, wxEXPAND, 0);
    SetSizer(sizer1);
    sizer1->Fit(this);
    Layout();
    Centre();
    // end wxGlade
}

BEGIN_EVENT_TABLE(WGetPassword, wxDialog)
    // begin wxGlade: WGetPassword::event_table
    EVT_TEXT_ENTER(myID_TEXTPASS, WGetPassword::OnTextPassEnter)
    // end wxGlade
END_EVENT_TABLE();

void WGetPassword::OnTextPassEnter(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_OK);
}

wxString WGetPassword::GetPass() const
{
    return textctrlPass->GetValue();
}

// wxGlade: add WGetPassword event handlers

WLegalNotice::WLegalNotice(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long WXUNUSED(style))
    : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE)
{
    // begin wxGlade: WLegalNotice::WLegalNotice
    labelText = new wxStaticText(this, wxID_ANY, wxEmptyString);
    bitmapWeb = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap);
    hyperlink1 = new wxHyperlinkCtrl(this, wxID_ANY, _("http://panthema.net/2009/cryptote/"), _("http://panthema.net/2009/cryptote/"), wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxHL_CONTEXTMENU | wxHL_ALIGN_LEFT);
    buttonOK = new wxButton(this, wxID_OK, _("OK"));

    set_properties();
    do_layout();
    // end wxGlade

    labelText->SetLabel(_("The password you just entered will be hashed and used to encrypt all subfiles\n"
                          "in the container. If you lose, forget or misspelled the password there is\n"
                          "NO KNOWN METHOD TO RETRIEVE YOUR SENSITIVE DATA WITHIN\n"
                          "THE NEXT 100 YEARS! If there were the cryptographic protection would be\n"
                          "pointless.\n"
                          "\n"
                          "Note too that this program is BETA SOFTWARE and free under the GNU\n"
                          "General Public License. As such it comes with NO WARRANTY. This means\n"
                          "if it contains some unknown bug or error which screws up your irreplaceable\n"
                          "password file, then I (the author) am not liable. You (re)acknowledge these\n"
                          "terms by pressing the OK button below.\n"
                          "\n"
                          "Don't be too troubled though, I use the program for my own password lists and\n"
                          "try to keep it bug-free. However, I had to point out this legal issue, which you\n"
                          "can find in virtually any software license agreement. If you find a bug, please\n"
                          "report it on"));

    #include "art/web-16.h"
    bitmapWeb->SetBitmap(wxBitmapFromMemory(web_16_png));

    // Setup countdown

    countdown = 15;

    mytimer.SetOwner(this);
    mytimer.Start(1000, wxTIMER_CONTINUOUS);

    buttonOK->SetLabel(wxString::Format(wxPLURAL("Wait %d second ...", "Wait %d seconds ...", countdown), countdown));

    buttonpressed = 0;

    Layout();
    GetSizer()->Fit(this);
    Centre();
}

void WLegalNotice::set_properties()
{
    // begin wxGlade: WLegalNotice::set_properties
    SetTitle(_("Legal Notice - CryptoTE"));
    // end wxGlade
}

void WLegalNotice::do_layout()
{
    // begin wxGlade: WLegalNotice::do_layout
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer3 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* label1 = new wxStaticText(this, wxID_ANY, _("Legal Notice"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    label1->SetFont(wxFont(14, wxDEFAULT, wxNORMAL, wxNORMAL, 0, wxT("")));
    sizer2->Add(label1, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 8);
    sizer2->Add(labelText, 1, wxLEFT | wxRIGHT | wxTOP | wxEXPAND, 8);
    sizer3->Add(bitmapWeb, 0, wxLEFT | wxRIGHT | wxBOTTOM | wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL, 4);
    sizer3->Add(hyperlink1, 0, wxRIGHT | wxBOTTOM | wxEXPAND | wxALIGN_CENTER_VERTICAL, 4);
    sizer2->Add(sizer3, 0, wxTOP | wxALIGN_CENTER_HORIZONTAL, 2);
    sizer1->Add(sizer2, 1, wxALL | wxEXPAND, 16);
    wxStaticLine* staticline1 = new wxStaticLine(this, wxID_ANY);
    sizer1->Add(staticline1, 0, wxEXPAND, 0);
    sizer1->Add(buttonOK, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 6);
    SetSizer(sizer1);
    sizer1->Fit(this);
    Layout();
    Centre();
    // end wxGlade
}

BEGIN_EVENT_TABLE(WLegalNotice, wxDialog)
    // begin wxGlade: WLegalNotice::event_table
    EVT_BUTTON(wxID_OK, WLegalNotice::OnButtonOK)
    // end wxGlade
    EVT_TIMER(wxID_ANY, WLegalNotice::OnTimer)
    EVT_CLOSE(WLegalNotice::OnClose)
END_EVENT_TABLE();

void WLegalNotice::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
    if (countdown == 0)
    {
        EndModal(wxID_OK);
    }
    else
    {
        buttonpressed = countdown;

        if (buttonpressed < countdown + 3 && buttonpressed >= countdown)
            buttonOK->SetLabel(wxString::Format(wxPLURAL("Wait %d second (Yes, I know it sucks. Sorry) ...", "Wait %d seconds (Yes, I know it sucks. Sorry) ...", countdown), countdown));
        else
            buttonOK->SetLabel(wxString::Format(wxPLURAL("Wait %d second ...", "Wait %d seconds ...", countdown), countdown));

        Layout();
    }
}

void WLegalNotice::OnClose(wxCloseEvent& event)
{
    event.Veto();
}

void WLegalNotice::OnTimer(wxTimerEvent& WXUNUSED(event))
{
    if (countdown > 1)
    {
        --countdown;

        if (buttonpressed < countdown + 3 && buttonpressed >= countdown)
            buttonOK->SetLabel(wxString::Format(wxPLURAL("Wait %d second (Yes, I know it sucks. Sorry) ...", "Wait %d seconds (Yes, I know it sucks. Sorry) ...", countdown), countdown));
        else
            buttonOK->SetLabel(wxString::Format(wxPLURAL("Wait %d second ...", "Wait %d seconds ...", countdown), countdown));

        Layout();
    }
    else if (countdown == 1)
    {
        countdown = 0;
        mytimer.Stop();

        buttonOK->SetLabel(_("OK"));
        Layout();
    }
    else {
        mytimer.Stop();
    }
}

/******************************************************************************/
