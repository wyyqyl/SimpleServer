#pragma once


// MyButton

class MyButton : public CButton
{
	DECLARE_DYNAMIC(MyButton)

public:
	MyButton();
	virtual ~MyButton();
	void setName(char *);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void handleClick(char *, UINT nFlags, CPoint point);
private:
	char name[20];
};


