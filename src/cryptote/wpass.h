// -*- C++ -*- generated by wxGlade 0.6.3 on Tue Mar 11 23:21:53 2008
// $Id$

#include <wx/wx.h>
// begin wxGlade: ::dependencies
#include <wx/statline.h>
// end wxGlade
#include "imaglbox.h"

#ifndef WPASS_H
#define WPASS_H

// begin wxGlade: ::extracode
// end wxGlade

class WPasswordList : public wxDialog
{
public:
    // begin wxGlade: WPasswordList::ids
    enum {
        myID_CHANGE = wxID_HIGHEST + 1000
    };
    // end wxGlade

    WPasswordList(class WCryptoTE* parent, int id=wxID_ANY, const wxString& title=wxEmptyString, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

protected:

    /// Reference to main window
    class WCryptoTE* wmain;

    /// Update listctrl
    void	ReinsertList();

private:
    // begin wxGlade: WPasswordList::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: WPasswordList::attributes
    wxSimpleImageListBox* listbox;
    wxButton* buttonAdd;
    wxButton* buttonChange;
    wxButton* buttonRemove;
    wxButton* buttonOK;
    // end wxGlade

public:
    virtual void OnButtonAdd(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnButtonChange(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnButtonRemove(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnButtonOK(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnListboxSelectionChanged(wxCommandEvent &event); // wxGlade: <event_handler>

protected:
    DECLARE_EVENT_TABLE();

}; // wxGlade: end class

class WSetPassword : public wxDialog
{
public:
    // begin wxGlade: WSetPassword::ids
    enum {
        myID_TEXTPASS = wxID_HIGHEST + 1002,
        myID_TEXTVERIFY = wxID_HIGHEST + 1004
    };
    // end wxGlade

    WSetPassword(wxWindow* parent, const wxString& filename, int id=wxID_ANY, const wxString& title=wxEmptyString, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

    /// Return pass after user enters it.
    wxString	GetPass() const;

private:
    // begin wxGlade: WSetPassword::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: WSetPassword::attributes
    wxStaticText* labelQuery;
    wxTextCtrl* textctrlPass;
    wxGauge* gaugeStrength;
    wxTextCtrl* textctrlVerify;
    wxButton* buttonOK;
    wxButton* buttonCancel;
    // end wxGlade

    int		state;
    wxString	pass;

    wxSizer*	sizerGauge;
    wxSizer*	sizerVerify;

    DECLARE_EVENT_TABLE();

public:
    virtual void OnTextPassEnter(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnTextPass(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnTextVerifyEnter(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnButtonOK(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class

class WGetPassword : public wxDialog
{
public:
    // begin wxGlade: WGetPassword::ids
    enum {
        myID_TEXTPASS = wxID_HIGHEST + 1006
    };
    // end wxGlade

    WGetPassword(wxWindow* parent, const wxString& filename, int id=wxID_ANY, const wxString& title=wxEmptyString, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

    /// Return pass after user enters it.
    wxString	GetPass() const;

private:
    // begin wxGlade: WGetPassword::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: WGetPassword::attributes
    wxStaticText* labelQuery;
    wxTextCtrl* textctrlPass;
    wxButton* buttonOK;
    wxButton* buttonCancel;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnTextPassEnter(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class

#endif // WPASS_H
