/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#pragma once


// CPatternEditor, the pattern editor class

#include "Common.h"
#include "PatternEditorTypes.h"


// Row color cache
struct RowColorInfo_t {
	COLORREF Note;
	COLORREF Instrument;
	COLORREF Volume;
	COLORREF Effect;
	COLORREF Back;
	COLORREF Shaded;
};

// External classes
class CFamiTrackerDoc;
class CFamiTrackerView;
class CMainFrame;

// CPatternEditor
class CPatternEditor {
	// Public methods
public:
	CPatternEditor();
	~CPatternEditor();

	void ApplyColorScheme();
	void SetDocument(CFamiTrackerDoc *pDoc, CFamiTrackerView *pView);
	void SetWindowSize(int width, int height);
	void ResetCursor();

	// Invalidation
	void InvalidatePatternData();
	void InvalidateCursor();
	void InvalidateBackground();
	void InvalidateHeader();

	// Drawing
	void DrawScreen(CDC *pDC, CFamiTrackerView *pView);	// Draw pattern area
	void DrawMeters(CDC *pDC);							// Draw channel meters
	void CreateBackground(CDC *pDC);					// Create off-screen buffers

	bool CursorUpdated();								// Update cursor state, returns true if erase is needed
	void UpdatePatternLength();							// Read pattern length
	void SetFocus(bool bFocus);

	CRect GetActiveRect() const;
	CRect GetHeaderRect() const;
	CRect GetPatternRect() const;
	CRect GetUnbufferedRect() const;
	CRect GetInvalidatedRect() const;

	// // //

	// Cursor movement
	void MoveDown(int Step);
	void MoveUp(int Step);
	void MoveLeft();
	void MoveRight();
	void MoveToTop();
	void MoveToBottom();
	void NextChannel();
	void PreviousChannel();
	void FirstChannel();
	void LastChannel();
	void MoveChannelLeft();
	void MoveChannelRight();
	void OnHomeKey();
	void OnEndKey();

	void MoveToRow(int Row);
	void MoveToFrame(int Frame);
	void MoveToChannel(int Channel);
	void MoveToColumn(int Column);
	void NextFrame();
	void PreviousFrame();

	void ScrollLeft();
	void ScrollRight();
	void ScrollNextChannel();
	void ScrollPreviousChannel();

	// Cursor state
	int GetFrame() const;
	int GetChannel() const;
	int GetRow() const;
	int GetColumn() const;

	// Mouse
	void OnMouseDown(const CPoint &point);
	void OnMouseUp(const CPoint &point);
	bool OnMouseHover(UINT nFlags, const CPoint &point);
	bool OnMouseNcMove();
	void OnMouseMove(UINT nFlags, const CPoint &point);
	void OnMouseDblClk(const CPoint &point);
	void OnMouseScroll(int Delta);
	void OnMouseRDown(const CPoint &point);

	bool CancelDragging();
	void CancelSelection();

	bool IsOverHeader(const CPoint &point) const;
	bool IsOverPattern(const CPoint &point) const;
	bool IsInsidePattern(const CPoint &point) const;
	bool IsInsideRowColumn(const CPoint &point) const;
	int GetChannelAtPoint(int PointX) const;

	// Edit: Copy & paste, selection
	CPatternClipData *CopyEntire() const;
	CPatternClipData *Copy() const;
	void Cut();
	void PasteEntire(const CPatternClipData *pClipData);
	void Paste(const CPatternClipData *pClipData);
	void PasteMix(const CPatternClipData *pClipData);

	bool IsSelecting() const;
	void SelectChannel();
	void SelectAllChannels();
	void SelectAll();

	// Various
	void GetVolumeColumn(CString &str) const;

	// Settings
	void SetHighlight(int Rows, int SecondRows);
	void SetFollowMove(bool bEnable);

	bool IsPlayCursorVisible() const;

	// Scrolling
	void AutoScroll(const CPoint &point, UINT nFlags);
	bool ScrollTimerCallback();
	void OnVScroll(UINT nSBCode, UINT nPos);
	void OnHScroll(UINT nSBCode, UINT nPos);

	// Selection
	void SetBlockStart();
	void SetBlockEnd();
	CSelection GetSelection() const;
	void SetSelection(const CSelection &selection);

	void DragPaste(const CPatternClipData *pClipData, const CSelection *pDragTarget, bool bMix);

	// OLE support
	void BeginDrag(const CPatternClipData *pClipData);
	void EndDrag();
	bool PerformDrop(const CPatternClipData *pClipData, bool bCopy, bool bCopyMix);
	void UpdateDrag(const CPoint &point);

#ifdef _DEBUG
	void DrawLog(CDC *pDC);
#endif

	// Public class methods
public:
	static int GetSelectColumn(int Column);

	// Private methods
private:

	// Layout
	bool CalculatePatternLayout();
	void CalcLayout();
	int GetCurrentPatternLength(unsigned int Frame) const;

	// Main draw methods
	void PerformFullRedraw(CDC *pDC);
	void PerformQuickRedraw(CDC *pDC);
	void DrawUnbufferedArea(CDC *pDC);
	void DrawHeader(CDC *pDC);

	// Helper draw methods
	void MovePatternArea(CDC *pDC, int FromRow, int ToRow, int NumRows) const;
	void ScrollPatternArea(CDC *pDC, int Rows) const;
	void ClearRow(CDC *pDC, int Line) const;
	void PrintRow(CDC *pDC, int Row, int Line, int Frame) const;
	void DrawRow(CDC *pDC, int Row, int Line, int Frame, bool bPreview) const;
	void DrawCell(CDC *pDC, int PosX, int Column, int Channel, bool bInvert, stChanNote *pNoteData, RowColorInfo_t *pColorInfo) const;
	void DrawChar(CDC *pDC, int x, int y, TCHAR c, COLORREF Color) const;

	// Other drawing
	void DrawChannelStates(CDC *pDC);
	void DrawRegisters(CDC *pDC);

	// Scrolling
	void UpdateVerticalScroll();
	void UpdateHorizontalScroll();

	// Translation
	int  GetRowAtPoint(int PointY) const;
	int  GetColumnAtPoint(int PointX) const;
	int  GetCursorStartColumn(int Column) const;
	int  GetCursorEndColumn(int Column) const;
	int	 GetChannelColumns(int Channel) const;
	int  GetSelectedTrack() const;
	int	 GetChannelCount() const;
	int	 RowToLine(int Row) const;

	CCursorPos GetCursorAtPoint(const CPoint &point) const;

	// Selection methods
	void SetSelectionStart();
	void UpdateSelection();

	void SetSelectionStart(const CCursorPos &start);
	void SetSelectionEnd(const CCursorPos &end);

	void BeginMouseSelection(const CPoint &point);
	void ContinueMouseSelection(const CPoint &point);

	// Editing
	void IncreaseEffectColumn(int Channel);
	void DecreaseEffectColumn(int Channel);

	// Keys
	bool IsShiftPressed() const;
	bool IsControlPressed() const;

	// Mouse
	void OnMouseDownHeader(const CPoint &point);
	void OnMouseDownPattern(const CPoint &point);

	// Main frame
	CMainFrame *GetMainFrame() const;	

public:
	// Public consts
	static const int HEADER_HEIGHT;

	// Colors
	static const COLORREF ROW_PLAY_COLOR = 0x400050;

private:
	// Private consts
	static LPCTSTR DEFAULT_HEADER_FONT;
	static const int DEFAULT_FONT_SIZE;
	static const int DEFAULT_HEADER_FONT_SIZE;

	static const int HEADER_CHAN_START;
	static const int HEADER_CHAN_HEIGHT;
	static const int ROW_COLUMN_WIDTH;
	static const int ROW_HEIGHT;
	static const int CHANNEL_WIDTH;

	// Variables
private:
	CFamiTrackerDoc	 *m_pDocument;
	CFamiTrackerView *m_pView;

	// GDI objects
	CDC		*m_pPatternDC;
	CDC		*m_pHeaderDC;
	CBitmap *m_pPatternBmp;
	CBitmap	*m_pHeaderBmp;
	CFont	m_fontHeader;
	CFont	m_fontPattern;
	CFont	m_fontCourierNew;

	// Window
	int		m_iWinWidth;					// Window height & width
	int		m_iWinHeight;

	// Flags
	bool	m_bPatternInvalidated;
	bool	m_bCursorInvalidated;
	bool	m_bBackgroundInvalidated;
	bool	m_bHeaderInvalidated;
	bool	m_bSelectionInvalidated;

	// Draw state variables
	int		m_iCenterRow;					// The row in the middle of the editor, will always point to a valid row in current frame
	int		m_iCurrentFrame;				// Selected frame
	
	int		m_iPatternLength;				// Pattern length of selected frame
	int		m_iPrevPatternLength;			// Pattern length of previous frame
	int		m_iNextPatternLength;			// Pattern length of next frame

	// Previous draw state
	int		m_iLastCenterRow;				// Previous center row
	int		m_iLastFrame;					// Previous frame
	int		m_iLastFirstChannel;			// Previous first visible channel
	int		m_iLastPlayRow;					// Previous play row

	// Play cursor
	int		m_iPlayRow;
	int		m_iPlayFrame;

//	CPatternEditorLayout m_Layout;

	// Pattern layout
	int		m_iPatternWidth;				// Width of channels in pattern area
	int		m_iPatternHeight;				// Height of channels in pattern area
	int		m_iLinesVisible;				// Number of lines visible on screen (may include one incomplete line)
	int		m_iLinesFullVisible;			// Number of lines full visible on screen
	int		m_iChannelsVisible;				// Number of channels visible on screen (may include one incomplete channel)
	int		m_iChannelsFullVisible;			// Number of channels full visible on screen
	int		m_iFirstChannel;				// First drawn channel
	int		m_iRowHeight;					// Height of each row in pixels
	int		m_iPatternFontSize;				// Size of pattern font

	int		m_iChannelWidths[MAX_CHANNELS];	// Cached width in pixels of each channel
	int		m_iChannelOffsets[MAX_CHANNELS];// Cached x position of channels
	int		m_iColumns[MAX_CHANNELS];		// Cached number of columns in each channel

	// Drawing (TODO remove these)
	int		m_iDrawCursorRow;
	int		m_iDrawFrame;

	// Settings
	bool	m_bFollowMode;					// Follow mode enable/disable
	bool	m_bHasFocus;					// Pattern editor has focus
	int		m_iHighlight;					// Pattern highlight settings
	int		m_iHighlightSecond;

	// Colors
	COLORREF m_colEmptyBg;
	COLORREF m_colSeparator;
	COLORREF m_colHead1;
	COLORREF m_colHead2;
	COLORREF m_colHead3;
	COLORREF m_colHead4;

	// // //

	int		m_iMouseHoverChan;
	int		m_iMouseHoverEffArrow;

	// Cursor position
	CCursorPos m_cpCursorPos;

	// Selection
	bool	m_bSelecting;			// Selection is active
	bool	m_bCurrentlySelecting;	// TODO remove this
	bool	m_bDragStart;			// Indicates that drag & drop is being initiated
	bool	m_bDragging;			// Drag & drop is active
	bool	m_bFullRowSelect;		// Enable full row selection

	// Mouse
	bool	m_bMouseActive;			// Indicates that mouse activity is in progess by the user
	int		m_iChannelPushed;		// Pushed channel state
	bool	m_bChannelPushed;

	CSelection m_selection;
	CCursorPos m_cpSelCursor;

	// Drag
	CSelection m_selDrag;
	CCursorPos m_cpDragPoint;

	CPoint m_ptSelStartPoint;

	// Numbers of pixels until selection is initiated
	int		m_iDragThresholdX;
	int		m_iDragThresholdY;

	// OLE support
	int		m_iDragChannels;
	int		m_iDragRows;
	int		m_iDragStartCol;
	int		m_iDragEndCol;

	int		m_iDragOffsetChannel;
	int		m_iDragOffsetRow;

	// Scrolling
	CPoint	m_ptScrollMousePos;
	UINT	m_nScrollFlags;
	scroll_t m_iScrolling;
	int		m_iCurrentHScrollPos;

	// Benchmarking
	mutable int m_iRedraws;
	mutable int m_iFullRedraws;
	mutable int m_iQuickRedraws;
	mutable int m_iHeaderRedraws;
	mutable int m_iPaints;
	mutable int m_iErases;
	mutable int m_iBuffers;
	mutable int m_iCharsDrawn;
};
