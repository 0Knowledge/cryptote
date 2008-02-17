// -*- C++ -*- generated by wxGlade 0.6.2 on Sun Feb 17 09:50:54 2008
// $Id$

#include "ptwnew.h"
#include "pgwmain.h"

#include "tools.h"

// begin wxGlade: ::extracode
// end wxGlade

PTWNew::PTWNew(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long WXUNUSED(style))
    : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxTHICK_FRAME)
{
    // begin wxGlade: PTWNew::PTWNew
    sizer2_staticbox = new wxStaticBox(this, -1, _("New Password"));
    textctrlDescription = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    textctrlPass = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    buttonGenPass = new wxButton(this, myID_GENPASS, _("Use Password &Generator"));
    const wxString choiceSpeed_choices[] = {
        _("Very Urgent (ask within 30 sec)"),
        _("Urgent (ask within 1 min)"),
        _("Important (ask within 3 min)")
    };
    choiceSpeed = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choiceSpeed_choices, 0);
    buttonAdd = new wxButton(this, wxID_OK, _("&Add to Learning List"));
    buttonCancel = new wxButton(this, wxID_CANCEL, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade
}

void PTWNew::set_properties()
{
    // begin wxGlade: PTWNew::set_properties
    SetTitle(_("Learn New Password"));
    textctrlDescription->SetToolTip(_("Enter any text to identify the password."));
    textctrlDescription->SetFocus();
    textctrlPass->SetFont(wxFont(10, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));
    textctrlPass->SetToolTip(_("Enter password you are going to be learning."));
    choiceSpeed->SetSelection(1);
    // end wxGlade
}

void PTWNew::do_layout()
{
    // begin wxGlade: PTWNew::do_layout
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer4 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer2 = new wxStaticBoxSizer(sizer2_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer3 = new wxFlexGridSizer(4, 2, 0, 0);
    wxStaticText* label1 = new wxStaticText(this, wxID_ANY, _("Description:"));
    sizer3->Add(label1, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(textctrlDescription, 0, wxALL|wxEXPAND, 6);
    wxStaticText* label2 = new wxStaticText(this, wxID_ANY, _("Password:"));
    sizer3->Add(label2, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(textctrlPass, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 6);
    sizer3->Add(5, 5, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(buttonGenPass, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_CENTER_VERTICAL, 6);
    wxStaticText* label3 = new wxStaticText(this, wxID_ANY, _("Tutoring Speed:"));
    sizer3->Add(label3, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    sizer3->Add(choiceSpeed, 0, wxLEFT|wxRIGHT|wxBOTTOM, 6);
    sizer3->AddGrowableCol(1);
    sizer2->Add(sizer3, 1, wxEXPAND, 0);
    sizer1->Add(sizer2, 1, wxALL|wxEXPAND, 6);
    sizer4->Add(buttonAdd, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    sizer4->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 0);
    sizer4->Add(buttonCancel, 0, wxALL, 6);
    sizer1->Add(sizer4, 0, wxEXPAND, 0);
    SetSizer(sizer1);
    sizer1->Fit(this);
    Layout();
    Centre();
    // end wxGlade
}

BEGIN_EVENT_TABLE(PTWNew, wxDialog)
    // begin wxGlade: PTWNew::event_table
    EVT_BUTTON(myID_GENPASS, PTWNew::OnButtonGenPass)
    EVT_BUTTON(wxID_OK, PTWNew::OnButtonOK)
    // end wxGlade
END_EVENT_TABLE();

void PTWNew::OnButtonGenPass(wxCommandEvent& WXUNUSED(event))
{
    PGWMain dlg(this, false);

    if (dlg.ShowModal() == wxID_OK)
    {
	textctrlPass->SetValue( dlg.GetSelectedPassword() );
    }
}

void PTWNew::OnButtonOK(wxCommandEvent& WXUNUSED(event))
{
    // Fill in PassEntry structure

    passentry.description = textctrlDescription->GetValue();
    if (passentry.description.IsEmpty()) {
	passentry.description = _("<no description>");
    }

    passentry.passtext = textctrlPass->GetValue();
    if (passentry.passtext.IsEmpty())
    {
	wxLogError(_("No password entered."));
	return;
    }

    passentry.ctime.SetToCurrent();
    passentry.ltime.SetToCurrent();
    passentry.timespent = 0;
    passentry.rights = 0;
    passentry.wrongs = 0;
    passentry.revealed = 0;
    passentry.scores.clear();

    // Hide password
    textctrlPass->Clear();

    EndModal(wxID_OK);
#if 0
    WAskPassword dlg(passentry, this);

    if (dlg.ShowModal() == wxID_OK)
    {
	EndModal(wxID_OK);
    }
    else
    {
	textctrlPass->SetValue(passentry.passtext);
    }
#endif
}

// wxGlade: add PTWNew event handlers
