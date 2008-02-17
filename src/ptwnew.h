// -*- C++ -*- generated by wxGlade 0.6.2 on Sun Feb 17 09:50:54 2008
// $Id$

#include <wx/wx.h>
#include <wx/image.h>

// begin wxGlade: ::dependencies
// end wxGlade

#include "ptwmain.h"

#ifndef PTWNEW_H
#define PTWNEW_H

// begin wxGlade: ::extracode
// end wxGlade

class PTWNew : public wxDialog
{
public:
    // begin wxGlade: PTWNew::ids
    enum {
        myID_GENPASS = wxID_HIGHEST + 1000
    };
    // end wxGlade

    PTWNew(wxWindow* parent, int id=wxID_ANY, const wxString& title=wxEmptyString, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

public:

    /// Password entry with all fields, this is later added to the list of
    /// passes to learn.
    class	PTPassEntry	passentry;

private:
    // begin wxGlade: PTWNew::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PTWNew::attributes
    wxStaticBox* sizer2_staticbox;
    wxTextCtrl* textctrlDescription;
    wxTextCtrl* textctrlPass;
    wxButton* buttonGenPass;
    wxChoice* choiceSpeed;
    wxButton* buttonAdd;
    wxButton* buttonCancel;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnButtonGenPass(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnButtonOK(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class

#endif // PTWNEW_H
