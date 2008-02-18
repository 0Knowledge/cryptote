// -*- C++ -*- generated by wxGlade 0.6.3 on Sun Feb 17 14:50:04 2008
// $Id$

#include "ptwstats.h"
#include "ptwmain.h"
#include "ptwquery.h"

#include "tools.h"

// begin wxGlade: ::extracode
// end wxGlade

PTWStats::PTWStats(struct PTPassEntry& _passentry, wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long WXUNUSED(style))
    : wxDialog(parent, id, title, pos, size, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxTHICK_FRAME),
      passentry(_passentry)
{
    // begin wxGlade: PTWStats::PTWStats
    sizer5_staticbox = new wxStaticBox(this, -1, _("Statistics"));
    sizer2_staticbox = new wxStaticBox(this, -1, _("Basic Settings"));
    textctrlDescription = new wxTextCtrl(this, wxID_ANY, wxEmptyString);
    textctrlPassword = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    buttonReveal = new wxToggleButton(this, myID_REVEAL, _("Reveal"));
    buttonQueryNow = new wxButton(this, myID_QUERYNOW, _("&Query Password Now"));
    labelCTime = new wxStaticText(this, wxID_ANY, _("2008-01-18 08:44"));
    labelLastQueried = new wxStaticText(this, wxID_ANY, _("2008-01-18 10:44"));
    labelNextQuery = new wxStaticText(this, wxID_ANY, _("in 20m 12s"));
    labelTimeSpent = new wxStaticText(this, wxID_ANY, _("1m 40s"));
    labelCorrect = new wxStaticText(this, wxID_ANY, _("2 times"));
    labelIncorrect = new wxStaticText(this, wxID_ANY, _("4 times"));
    labelRevealed = new wxStaticText(this, wxID_ANY, _("1 time"));
    bitmapScores = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap);
    labelScores = new wxStaticText(this, wxID_ANY, _("1, 2, 56, 20, 50, 40, 34"));
    buttonOK = new wxButton(this, wxID_OK, wxEmptyString);
    buttonCancel = new wxButton(this, wxID_CANCEL, wxEmptyString);

    set_properties();
    do_layout();
    // end wxGlade

    UpdateValues();
}

void PTWStats::set_properties()
{
    // begin wxGlade: PTWStats::set_properties
    SetTitle(_("Password Statistics"));
    textctrlPassword->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    textctrlPassword->SetFont(wxFont(10, wxMODERN, wxNORMAL, wxNORMAL, 0, wxT("")));
    // end wxGlade
}

void PTWStats::do_layout()
{
    // begin wxGlade: PTWStats::do_layout
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* gridsizer7 = new wxGridSizer(1, 2, 0, 0);
    wxStaticBoxSizer* sizer5 = new wxStaticBoxSizer(sizer5_staticbox, wxVERTICAL);
    wxFlexGridSizer* gridsizer6 = new wxFlexGridSizer(9, 2, 0, 0);
    wxStaticBoxSizer* sizer2 = new wxStaticBoxSizer(sizer2_staticbox, wxVERTICAL);
    wxFlexGridSizer* gridsizer3 = new wxFlexGridSizer(3, 2, 0, 0);
    wxBoxSizer* sizer4 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* labelA1 = new wxStaticText(this, wxID_ANY, _("Description:"));
    gridsizer3->Add(labelA1, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer3->Add(textctrlDescription, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 6);
    wxStaticText* labelA2 = new wxStaticText(this, wxID_ANY, _("Password:"));
    gridsizer3->Add(labelA2, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    sizer4->Add(textctrlPassword, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 0);
    sizer4->Add(buttonReveal, 0, wxLEFT|wxALIGN_CENTER_VERTICAL|wxFIXED_MINSIZE, 6);
    gridsizer3->Add(sizer4, 1, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 6);
    gridsizer3->Add(5, 5, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer3->Add(buttonQueryNow, 0, wxLEFT|wxRIGHT|wxBOTTOM, 6);
    gridsizer3->AddGrowableCol(1);
    sizer2->Add(gridsizer3, 1, wxEXPAND, 0);
    sizer1->Add(sizer2, 0, wxALL|wxEXPAND, 6);
    wxStaticText* labelB1 = new wxStaticText(this, wxID_ANY, _("Created:"));
    gridsizer6->Add(labelB1, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer6->Add(labelCTime, 0, wxALL, 6);
    wxStaticText* labelB2 = new wxStaticText(this, wxID_ANY, _("Last Queried:"));
    gridsizer6->Add(labelB2, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer6->Add(labelLastQueried, 0, wxLEFT|wxRIGHT|wxBOTTOM, 6);
    wxStaticText* labelB3 = new wxStaticText(this, wxID_ANY, _("Next Query:"));
    gridsizer6->Add(labelB3, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer6->Add(labelNextQuery, 0, wxLEFT|wxRIGHT|wxBOTTOM, 6);
    wxStaticText* labelB4 = new wxStaticText(this, wxID_ANY, _("Time spent in Query:"));
    gridsizer6->Add(labelB4, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer6->Add(labelTimeSpent, 0, wxLEFT|wxRIGHT|wxBOTTOM, 6);
    wxStaticText* labelB5 = new wxStaticText(this, wxID_ANY, _("Correct Entries:"));
    gridsizer6->Add(labelB5, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer6->Add(labelCorrect, 0, wxLEFT|wxRIGHT|wxBOTTOM, 6);
    wxStaticText* labelB6 = new wxStaticText(this, wxID_ANY, _("Incorrect Entries:"));
    gridsizer6->Add(labelB6, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer6->Add(labelIncorrect, 0, wxLEFT|wxRIGHT|wxBOTTOM, 6);
    wxStaticText* labelB7 = new wxStaticText(this, wxID_ANY, _("Revealed:"));
    gridsizer6->Add(labelB7, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer6->Add(labelRevealed, 0, wxLEFT|wxRIGHT|wxBOTTOM, 6);
    wxStaticText* labelB8 = new wxStaticText(this, wxID_ANY, _("Score:"));
    gridsizer6->Add(labelB8, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer6->Add(bitmapScores, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 6);
    wxStaticText* labelB9 = new wxStaticText(this, wxID_ANY, _("Last 10 Scores:"));
    gridsizer6->Add(labelB9, 0, wxLEFT|wxRIGHT|wxBOTTOM|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer6->Add(labelScores, 0, wxLEFT|wxRIGHT|wxBOTTOM, 6);
    gridsizer6->AddGrowableCol(1);
    sizer5->Add(gridsizer6, 1, wxEXPAND, 0);
    sizer1->Add(sizer5, 1, wxLEFT|wxRIGHT|wxBOTTOM|wxEXPAND, 6);
    gridsizer7->Add(buttonOK, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 6);
    gridsizer7->Add(buttonCancel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 6);
    sizer1->Add(gridsizer7, 0, wxEXPAND, 0);
    SetSizer(sizer1);
    sizer1->Fit(this);
    Layout();
    Centre();
    // end wxGlade
}

BEGIN_EVENT_TABLE(PTWStats, wxDialog)
    // begin wxGlade: PTWStats::event_table
    EVT_TOGGLEBUTTON(myID_REVEAL, PTWStats::OnButtonReveal)
    EVT_BUTTON(myID_QUERYNOW, PTWStats::OnButtonQueryNow)
    // end wxGlade
END_EVENT_TABLE();

void PTWStats::OnButtonReveal(wxCommandEvent& WXUNUSED(event))
{
    if (buttonReveal->GetValue())
    {
	textctrlPassword->SetValue( passentry.passtext );
    }
    else
    {
	textctrlPassword->SetValue( wxString('*', passentry.passtext.size()) );
    }
}

void PTWStats::OnButtonQueryNow(wxCommandEvent& WXUNUSED(event))
{
    PTWQuery dlg(passentry, this);
    dlg.ShowModal();

    UpdateValues();
}

// wxGlade: add PTWStats event handlers

void PTWStats::UpdateValues()
{
    textctrlDescription->SetValue(passentry.description);

    if (buttonReveal->GetValue())
    {
	textctrlPassword->SetValue( passentry.passtext );
    }
    else
    {
	textctrlPassword->SetValue( wxString('*', passentry.passtext.size()) );
    }

    labelCTime->SetLabel(passentry.ctime.Format(_("%Y-%m-%d %H:%M:%S")));
    labelLastQueried->SetLabel(passentry.ltime.Format(_("%Y-%m-%d %H:%M:%S")));
    labelNextQuery->SetLabel(_("Unknown"));
    labelTimeSpent->SetLabel( wxTimeSpan(0,0,0,passentry.timespent).Format() );
    labelCorrect->SetLabel( wxString::Format(_("%u times"), passentry.rights) );
    labelIncorrect->SetLabel( wxString::Format(_("%u times"), passentry.wrongs) );
    labelRevealed->SetLabel( wxString::Format(_("%u times"), passentry.revealed) );
    
    bitmapScores->SetBitmap( MakeScoreBitmap() );

    wxString scores;
    for(unsigned int i = 0; i < 10; ++i)
    {
	if (passentry.scores.size() > i)
	{
	    if (!scores.IsEmpty()) scores += _T(", ");
	    scores += wxString::Format(_("%u"), passentry.scores[ passentry.scores.size() - 1 - i ]);
	}
    }

    labelScores->SetLabel(scores);
}

wxBitmap PTWStats::MakeScoreBitmap()
{
    const int height = 16;
    const int width = 120;

    wxBitmap bitmap(width, height);
    wxMemoryDC dc;
    dc.SelectObject(bitmap);

    dc.SetBackground(*wxBLACK_BRUSH);
    dc.Clear();

    double step = (double)(width-2) / (double)passentry.scores.size();

    // support points in gradient

    static const struct GradientPoint gradient[] =
	{
	    {   0, wxColour(  0, 242, 0) },
	    {   1, wxColour(194, 242, 0) },
	    {  10, wxColour(242, 214, 0) },
	    {  40, wxColour(242,   0, 0) }
	};

    for(unsigned int i = 0; i < passentry.scores.size(); ++i)
    {
	const int& score = passentry.scores[ passentry.scores.size() - 1 - i ];

	wxColour colour = InterpolateGradient(score, gradient, sizeof(gradient) / sizeof(gradient[0]));

	dc.SetBrush( wxBrush(colour) );
	dc.SetPen(*wxTRANSPARENT_PEN);

	int rectw = wxCoord(step)+1;
	if (i+1 == passentry.scores.size()) rectw = width-2 - wxCoord(i*step);

	dc.DrawRectangle( 1+wxCoord(i*step), 1, rectw, height-2);
    }

    return bitmap;
}
