// -*- C++ -*- generated by wxGlade 0.6.3 on Wed Feb 20 10:42:51 2008
// $Id$

#include "cewfind.h"
#include "cewmain.h"
#include "cewedit.h"

#include "tools.h"

// begin wxGlade: ::extracode
// end wxGlade

CEWFind::CEWFind(class CEWMain* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long WXUNUSED(style))
    : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxTHICK_FRAME),
      wmain(parent)
{
    // begin wxGlade: CEWFind::CEWFind
    sizer3_staticbox = new wxStaticBox(this, -1, _("Options"));
    const wxString *comboFind_choices = NULL;
    comboFind = new wxComboBox(this, myID_COMBO_FIND, wxT(""), wxDefaultPosition, wxDefaultSize, 0, comboFind_choices, wxCB_DROPDOWN|wxCB_DROPDOWN);
    labelReplace = new wxStaticText(this, wxID_ANY, _("Replace:"));
    const wxString *comboReplace_choices = NULL;
    comboReplace = new wxComboBox(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, comboReplace_choices, wxCB_DROPDOWN|wxCB_DROPDOWN);
    checkboxMatchWholeWord = new wxCheckBox(this, wxID_ANY, _("Match whole words only."));
    checkboxMatchCaseInsensitive = new wxCheckBox(this, wxID_ANY, _("Match case-sensitive."));
    checkboxMatchRegex = new wxCheckBox(this, wxID_ANY, _("Match as regular expression."));
    checkboxSearchBackwards = new wxCheckBox(this, wxID_ANY, _("Search backwards."));
    staticline1 = new wxStaticLine(this, wxID_ANY);
    buttonFind = new wxButton(this, wxID_FIND, wxEmptyString);
    buttonReplace = new wxButton(this, wxID_REPLACE, _("&Replace"));
    buttonReplaceAll = new wxButton(this, myID_REPLACEALL, _("Replace &All"));
    buttonClose = new wxButton(this, wxID_CLOSE, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

    ShowReplace(false);

    comboFind->SetFocus();
}

void CEWFind::set_properties()
{
    // begin wxGlade: CEWFind::set_properties
    SetTitle(_("Find & Replace - CryptoTE"));
    // end wxGlade
}

void CEWFind::do_layout()
{
    // begin wxGlade: CEWFind::do_layout
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer4 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer3 = new wxStaticBoxSizer(sizer3_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer2 = new wxFlexGridSizer(2, 2, 0, 0);
    wxStaticText* label1 = new wxStaticText(this, wxID_ANY, _("Find:"));
    sizer2->Add(label1, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer2->Add(comboFind, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 4);
    sizer2->Add(labelReplace, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 4);
    sizer2->Add(comboReplace, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 4);
    sizer2->AddGrowableRow(0);
    sizer2->AddGrowableRow(1);
    sizer2->AddGrowableCol(1);
    sizer1->Add(sizer2, 1, wxTOP|wxEXPAND, 4);
    sizer3->Add(checkboxMatchWholeWord, 0, wxALL, 2);
    sizer3->Add(checkboxMatchCaseInsensitive, 0, wxALL, 2);
    sizer3->Add(checkboxMatchRegex, 0, wxALL, 2);
    sizer3->Add(checkboxSearchBackwards, 0, wxALL, 2);
    sizer1->Add(sizer3, 0, wxALL|wxEXPAND, 4);
    sizer1->Add(staticline1, 0, wxEXPAND, 0);
    sizer4->Add(buttonFind, 0, wxALL, 4);
    sizer4->Add(buttonReplace, 0, wxALL, 4);
    sizer4->Add(buttonReplaceAll, 0, wxALL, 4);
    sizer4->Add(5, 5, 1, 0, 0);
    sizer4->Add(buttonClose, 0, wxALL, 4);
    sizer1->Add(sizer4, 0, wxEXPAND, 0);
    SetSizer(sizer1);
    sizer1->Fit(this);
    Layout();
    Centre();
    // end wxGlade
}

void CEWFind::ShowReplace(bool show)
{
    showreplace = show;

    if (!showreplace)
	SetTitle(_("Find - CryptoTE"));
    else
	SetTitle(_("Find & Replace - CryptoTE"));

    labelReplace->Show( showreplace );
    comboReplace->Show( showreplace );

    buttonReplace->Show( showreplace );
    buttonReplaceAll->Show( showreplace );

    Layout();
    GetSizer()->Fit(this);
    Layout();
    GetSizer()->Fit(this);
}

BEGIN_EVENT_TABLE(CEWFind, wxDialog)
    // begin wxGlade: CEWFind::event_table
    EVT_TEXT(myID_COMBO_FIND, CEWFind::OnComboTextSearch)
    EVT_BUTTON(wxID_FIND, CEWFind::OnButtonFind)
    EVT_BUTTON(wxID_REPLACE, CEWFind::OnButtonReplace)
    EVT_BUTTON(myID_REPLACEALL, CEWFind::OnButtonReplaceAll)
    EVT_BUTTON(wxID_ANY, CEWFind::OnButtonClose)
    // end wxGlade
END_EVENT_TABLE();

void CEWFind::OnComboTextSearch(wxCommandEvent& WXUNUSED(event))
{
    havefound = false;
}

void CEWFind::OnButtonFind(wxCommandEvent& WXUNUSED(event))
{
    wxString findtext = comboFind->GetValue();

    if (findtext.IsEmpty())
    {
	wmain->UpdateStatusBar( _("Nothing to search for.") );

	return;
    }

    { // Add text into combo box at beginning if it is not already in.

	int pos = comboFind->FindString(findtext);
	if (pos != wxNOT_FOUND) {
	    comboFind->Delete(pos);
	}

	comboFind->Insert(findtext, 0);
    }

    bool forward = !checkboxSearchBackwards->GetValue();

    CEWEdit* editctrl = wmain->editctrl;

    if (forward)
    {
	editctrl->SetTargetStart( editctrl->GetCurrentPos() );
	editctrl->SetTargetEnd(editctrl->GetLength());
    }
    else
    {
	editctrl->SetTargetStart( editctrl->GetCurrentPos() - 1 );
	editctrl->SetTargetEnd(0);
    }

    int flags = 0;
    if (checkboxMatchWholeWord->GetValue()) flags |= wxSTC_FIND_WHOLEWORD;
    if (checkboxMatchCaseInsensitive->GetValue()) flags |= wxSTC_FIND_MATCHCASE;
    if (checkboxMatchRegex->GetValue()) flags |= wxSTC_FIND_REGEXP;
    editctrl->SetSearchFlags(flags);

    int respos = editctrl->SearchInTarget(findtext);

    bool wrapped = false;

    if (respos < 0)
    {
	// wrap-around search
	wrapped = true;

	if (forward)
	{
	    editctrl->SetTargetStart(0);
	    editctrl->SetTargetEnd( editctrl->GetCurrentPos() );
	}
	else
	{
	    editctrl->SetTargetStart(editctrl->GetLength());
	    editctrl->SetTargetEnd( editctrl->GetCurrentPos() );
	}

	respos = editctrl->SearchInTarget(findtext);
    }

    bool found = false;
    if (respos >= 0)
    {
	found = true;
	int start = editctrl->GetTargetStart();
	int end = editctrl->GetTargetEnd();

	editctrl->EnsureVisible( editctrl->LineFromPosition(start) );
	editctrl->SetSelection(start, end);
    }

    if (found && !wrapped) {
	wmain->UpdateStatusBar( wxT("") );
    }
    else if (found && wrapped) {
	if (forward)
	    wmain->UpdateStatusBar( _("Search wrapped to beginning of document.") );
	else
	    wmain->UpdateStatusBar( _("Search wrapped to end of document.") );
    }
    else if (!found) {
	wmain->UpdateStatusBar( _("Search string not found in document.") );
    }

    havefound = found;
}

void CEWFind::OnButtonReplace(wxCommandEvent& event)
{
    wxString replacetext = comboReplace->GetValue();

    { // Add text into combo box at beginning if it is not already in.

	int pos = comboReplace->FindString(replacetext);
	if (pos != wxNOT_FOUND) {
	    comboReplace->Delete(pos);
	}

	comboReplace->Insert(replacetext, 0);
    }

    if (havefound)
    {
	wmain->editctrl->ReplaceTarget(replacetext);
    }

    OnButtonFind(event);
}

void CEWFind::OnButtonReplaceAll(wxCommandEvent& event)
{
    wxString replacetext = comboReplace->GetValue();

    { // Add text into combo box at beginning if it is not already in.

	int pos = comboReplace->FindString(replacetext);
	if (pos != wxNOT_FOUND) {
	    comboReplace->Delete(pos);
	}

	comboReplace->Insert(replacetext, 0);
    }

    if (!havefound) {
	OnButtonFind(event);
    }

    while(havefound)
    {
	wmain->editctrl->ReplaceTarget(replacetext);

	OnButtonFind(event);
    }
}

void CEWFind::OnButtonClose(wxCommandEvent& WXUNUSED(event))
{
    Hide();
}

// wxGlade: add CEWFind event handlers
