// -*- C++ -*- generated by wxGlade 0.6.3 on Tue Mar 11 23:21:53 2008
// $Id$

/*
 * CryptoTE v0.0.0
 * Copyright (C) 2008-2009 Timo Bingmann
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <wx/wx.h>
#include <wx/hyperlink.h>
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
        myID_PASSLIST = wxID_HIGHEST + 1000,
        myID_CHANGE = wxID_HIGHEST + 1001,
        myID_REMOVE = wxID_HIGHEST + 1003
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
        myID_TEXTPASS = wxID_HIGHEST + 1005,
        myID_TEXTVERIFY = wxID_HIGHEST + 1007,
        myID_TEXTDESCRIPTION = wxID_HIGHEST + 1009,
        myID_HYPERLINK_ADVICE = wxID_HIGHEST + 1010
    };
    // end wxGlade

    WSetPassword(class WCryptoTE* wmain, wxWindow* parent, const wxString& filename, int id=wxID_ANY, const wxString& title=wxEmptyString, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

    /// Return pass after user enters it.
    const wxString&	GetPass() const;

    /// Return description after user enters it.
    const wxString&	GetDescription() const;

    /// Set description for existing slots
    void		SetDescription(const wxString& desc);

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
    wxTextCtrl* textctrlDescription;
    wxHyperlinkCtrl* hyperlinkAdvice;
    wxButton* buttonOK;
    wxButton* buttonCancel;
    // end wxGlade

    int		state;
    wxString	pass;
    wxString	description;

    wxSizer*	sizerGauge;
    wxSizer*	sizerVerify;

    /// Reference to main window
    class WCryptoTE* wmain;

    DECLARE_EVENT_TABLE();

public:
    virtual void OnTextPassEnter(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnTextPass(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnTextVerifyEnter(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnButtonOK(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnHyperlinkAdvice(wxHyperlinkEvent& event);
}; // wxGlade: end class

class WGetPassword : public wxDialog
{
public:
    // begin wxGlade: WGetPassword::ids
    enum {
        myID_TEXTPASS = wxID_HIGHEST + 1011
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

class WLegalNotice : public wxDialog
{
public:
    // begin wxGlade: WLegalNotice::ids
    // end wxGlade

    WLegalNotice(wxWindow* parent, int id=wxID_ANY, const wxString& title=wxEmptyString, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    /// wxTimer object to update and ultimately enable the OK button
    wxTimer		mytimer;

    /// Current count down in seconds
    unsigned int	countdown;

    /// Display a differen text
    unsigned int	buttonpressed;

private:
    // begin wxGlade: WLegalNotice::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: WLegalNotice::attributes
    wxStaticText* labelText;
    wxStaticBitmap* bitmapWeb;
    wxHyperlinkCtrl* hyperlink1;
    wxButton* buttonOK;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnTimer(wxTimerEvent &event);
    virtual void OnClose(wxCloseEvent &event);
    virtual void OnButtonOK(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class

#endif // WPASS_H
