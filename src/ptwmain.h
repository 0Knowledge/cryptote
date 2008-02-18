// -*- C++ -*- generated by wxGlade 0.6.2 on Sun Feb 17 09:28:37 2008
// $Id$

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/datetime.h>
// begin wxGlade: ::dependencies
#include <wx/listctrl.h>
// end wxGlade

#include <vector>

#ifndef PTWMAIN_H
#define PTWMAIN_H

// begin wxGlade: ::extracode
// end wxGlade

/// Structure holding all information about a password begin learned.
struct PTPassEntry
{
    /// Description given by the user
    wxString	description;

    /// The password itself
    wxString	passtext;

    /// Creation time
    wxDateTime	ctime;

    /// Last time queried from user
    wxDateTime	ltime;

    /// Duration of all queries
    int		timespent;

    /// Total times queried and entered correctly
    int		rights;

    /// Total times entered incorrectly
    int		wrongs;

    /// Total times revealed
    int		revealed;

    /// Score of all queries in order. Score = 0 for perfect entry, bad entry
    /// and revealing add points.
    std::vector<int>	scores;
};

class PTWMain : public wxFrame
{
public:
    // begin wxGlade: PTWMain::ids
    enum {
        myID_PASSLIST = wxID_HIGHEST + 1000,
        myID_NEWPASS = wxID_HIGHEST + 1002,
        myID_ABOUT = wxID_HIGHEST + 1004,
        myID_HIDE = wxID_HIGHEST + 1006
    };
    // end wxGlade

    enum {
	myID_MENU_STATS = wxID_HIGHEST + 2000,
	myID_MENU_QUERY,
	myID_MENU_STOP,
	myID_MENU_ERASE
    };

    PTWMain(wxWindow* parent, int id=wxID_ANY, const wxString& title=wxEmptyString, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

protected:

    // *** PassEntry Management Functions ***

    typedef std::vector<struct PTPassEntry> passlist_type;

    /// List of all learned passwords
    std::vector<struct PTPassEntry> passlist;

    /// Load passlist from config file as encrypted strings
    void	loadPasslist();

    /// Save passlist into config file as encrypted strings
    void	savePasslist();

    /// Load passlist from decrypted wxFileConfig
    void	loadPasslistFrom(class wxConfigBase& cfg);

    /// Save passlist to wxFileConfig which will be encrypted
    void	savePasslistTo(class wxConfigBase& cfg);

    /// Append a single new pass entry to the wxListBox
    void	AppendPassEntry(struct PTPassEntry& pe);

    /// Update a pass entry in the wxListBox
    void	UpdatePassEntry(int ni);

private:
    // begin wxGlade: PTWMain::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: PTWMain::attributes
    wxStaticBox* sizer2_staticbox;
    wxListCtrl* listctrlPasslist;
    wxButton* buttonNewPass;
    wxButton* buttonAbout;
    wxButton* buttonHide;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void OnMenuStatistics(wxCommandEvent& event);
    virtual void OnMenuQuery(wxCommandEvent& event);
    virtual void OnMenuStop(wxCommandEvent& event);
    virtual void OnMenuErase(wxCommandEvent& event);

    virtual void OnPasslistItemActivated(wxListEvent &event); // wxGlade: <event_handler>
    virtual void OnPasslistItemRightClick(wxListEvent &event); // wxGlade: <event_handler>
    virtual void OnButtonNewPass(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnButtonAbout(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void OnButtonClose(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class

#endif // PTWMAIN_H
