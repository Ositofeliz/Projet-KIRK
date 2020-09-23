// **********************************
// **********************************
// Définition des KirkWidgets
// **********************************
// **********************************
#ifndef KIRKWIDGET_H
#define KIRKWIDGET_H
// **********************************
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QTimeEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QKeyEvent>
#include <cmath>
// **********************************
#include <iostream>
using namespace std;
// **********************************
// **********************************
// Options des widgets
class SETS
{
public:
	static constexpr int SidebarW = 300;
	static constexpr int TitleH = 40;
	static constexpr int Spacing = 5;
	static constexpr int Margin = 5;
	static constexpr int normalH = 30;
	static constexpr int NameW = 120;
	static constexpr int NameTrajW = 70;
	static constexpr int SpinBoxWidth = 150;
	static constexpr int TextEditH = 100;
	static constexpr int sepH = 2;
};
// **********************************
// **********************************
// KLabel Widget
class KLabel : public QLabel
{
	Q_OBJECT
public:
	// Destructeur
	~KLabel() {};
	// Constructeurs
	KLabel( QWidget *parent = nullptr )
		: QLabel( parent )
	{

		//setFixedHeight( SETS::boxHeight );
	};
	KLabel( QString text, QWidget *parent = nullptr )
		: QLabel( text, parent )
	{
		setAlignment( Qt::AlignRight | Qt::AlignVCenter );
		//setFixedHeight( SETS::boxHeight );
	};
	// ----------------
	// Getters & Setters
	void setRect( int x, int y, int w, int h )
	{
		setGeometry( x, y, w, h );

	};
	// ----------------
	// ----------------
};
// **********************************
// **********************************
// KLineEdit Widget
class KLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	// Destructeur
	~KLineEdit() {};
	// Constructeurs
	KLineEdit( QWidget *parent = nullptr )
		: QLineEdit( parent )
	{

		//setFixedHeight( SETS::boxHeight );
	};
	KLineEdit( QString text, QWidget *parent = nullptr )
		: QLineEdit( text, parent )
	{

		//setFixedHeight( SETS::boxHeight );
	};
	void setRect( int x, int y, int w, int h )
	{
		setGeometry( x, y, w, h );

	};
	// ----------------
	// ----------------
};
// **********************************
// **********************************
// KCoordEdit Widget
class KCoordEdit : public QLineEdit
{
	Q_OBJECT
private:
	bool _isLatitude = true;
	// ----------------
	// ----------------
public:
	// Destructeur
	~KCoordEdit() {};
	// Constructeurs
	KCoordEdit( QWidget *parent = nullptr )
		: QLineEdit( parent )
	{
		setInputMask( "000:00:00 A" );
		setText( "0000000N" );
	};
	// ----------------
	// Getters & Setters
	void setRect( int x, int y, int w, int h )
	{
		setGeometry( x, y, w, h );

	};
	void isLatitude() { _isLatitude = true; }
	void isLongitude() {
		_isLatitude = false;
		setText( "0000000E" );
	}
	// ----------------
	// ----------------
private:
	// Méthodes privées
	void modify( char direc, char car )
	{
		int cursorPos = cursorPosition();
		//cout << text().toStdString() << endl;
		QStringList list1 = text().split(" ", QString::SkipEmptyParts);
		QStringList list2 = list1.at(0).split(":", QString::SkipEmptyParts);
		short dd = list2.at(0).toShort() ;
		short mm = list2.at(1).toShort() ;
		short ss = list2.at(2).toShort() ;
		QString di = list1.at(1) ;
		//cout << dd << " - " << mm << " - " << ss << " - " << di.toStdString()  << endl;

		if ( cursorPos == 10 )
		{
			if ( _isLatitude )
			{
				if ( car == -1 )
				{
					if ( di == "N" ) di = "S";
					else di = "N";
				}
				else
				{
					if ( car == 'N' )
					{
						di = "N";
					}
					else if ( car == 'S' )
					{
						di = "S";
					}
				}
			}
			else
			{
				if ( car == -1 )
				{
					if ( di == "O" ) di = "E";
					else di = "O";
				}
				else
				{
					if ( car == 'O' )
					{
						di = "O";
					}
					else if ( car == 'E' )
					{
						di = "E";
					}
				}
			}
		}
		else if ( cursorPos <= 2 )
		{
			short inc = direc;
			for ( char it = 2-cursorPos; it > 0; it--) inc *=10;
			dd += inc;
			if ( dd < 0) dd = 0;
			else if ( dd > 180) dd = 180;
		}
		else if ( cursorPos == 4 || cursorPos == 5 )
		{
			short inc = direc; if ( cursorPos == 4 ) inc *= 10;
			mm += inc;
			if ( mm + inc < 0) mm = 0;
			else if ( mm > 60) mm = 60;
		}
		else if ( cursorPos == 7 || cursorPos == 8 )
		{
			short inc = direc; if ( cursorPos == 7 ) inc *= 10;
			ss += inc;
			if ( ss + inc < 0) ss = 0;
			else if ( ss > 60) ss = 60;
		}
		QString dds = QString::number( dd );
		while ( dds.size() < 3 ) dds.push_front("0");
		QString mms = QString::number( mm );
		while ( mms.size() < 2 ) mms.push_front("0");
		QString sss = QString::number( ss );
		while ( sss.size() < 2 ) sss.push_front("0");

		setText( dds+":"+mms+":"+sss+" "+di );
		setCursorPosition( cursorPos );
	};
	void keyPressEvent(QKeyEvent *keyevent) override
	{
		switch ( keyevent->key() )
		{
			case Qt::Key_Up :
				modify(1, -1);
			break;
			case Qt::Key_Down :
				modify(-1, -1);
			break;
			case Qt::Key_Left :
				cursorBackward(false);
			break;
			case Qt::Key_Right :
				cursorForward(false);
			break;
			case Qt::Key_0 :
				modify(0, 0);
			break;
			case Qt::Key_1 :
				modify(0, 1);
			break;
			case Qt::Key_2 :
				modify(0, 2);
			break;
			case Qt::Key_3 :
				modify(0, 3);
			break;
			case Qt::Key_4 :
				modify(0, 4);
			break;
			case Qt::Key_5 :
				modify(0, 5);
			break;
			case Qt::Key_6 :
				modify(0, 6);
			break;
			case Qt::Key_7 :
				modify(0, 7);
			break;
			case Qt::Key_8 :
				modify(0, 8);
			break;
			case Qt::Key_9 :
				modify(0, 9);
			break;
			case Qt::Key_N :
				modify(0, 'N');
			break;
			case Qt::Key_S :
				modify(0, 'S');
			break;
			case Qt::Key_O :
				modify(0, 'O');
			break;
			case Qt::Key_E :
				modify(0, 'E');
			break;
			default :
			break;
		}
	}
	// ----------------
	// ----------------
};
// **********************************
// **********************************
// KTextEdit Widget
class KTextEdit : public QTextEdit
{
	Q_OBJECT
public:
	// Destructeur
	~KTextEdit() {};
	// Constructeurs
	KTextEdit( QWidget *parent = nullptr )
		: QTextEdit( parent )
	{
	};
	KTextEdit( QString text, QWidget *parent = nullptr )
		: QTextEdit( text, parent )
	{
	};
	// ----------------
	// Getters & Setters
	void setRect( int x, int y, int w, int h )
	{
		setGeometry( x, y, w, h );

	};
	// ----------------
	// ----------------
};
// **********************************
// **********************************
// KTextEdit Widget
class KPushButton : public QPushButton
{
	Q_OBJECT
public:
	// Destructeur
	~KPushButton() {};
	// Constructeurs
	KPushButton( QWidget *parent = nullptr )
		: QPushButton( parent )
	{
		setIconSize( {height()-10, height()-10} );
	};
	KPushButton( QString text, QWidget *parent = nullptr )
		: QPushButton( text, parent )
	{
		setIconSize( {height()-10, height()-10} );
	};
	// ----------------
	// Getters & Setters
	void setRect( int x, int y, int w, int h )
	{
		setGeometry( x, y, w, h );

	};
	// ----------------
	// ----------------
};
// **********************************
// **********************************
// KComboBox Widget
class KComboBox : public QComboBox
{
	Q_OBJECT
public:
	// Destructeur
	~KComboBox() {};
	// Constructeurs
	KComboBox( QWidget *parent = nullptr )
		: QComboBox( parent )
	{
	};
	KComboBox( QStringList combo, QWidget *parent = nullptr )
		: QComboBox( parent )
	{
		addItems( combo );
	};
	// ----------------
	// Getters & Setters
	void setRect( int x, int y, int w, int h )
	{
		setGeometry( x, y, w, h );

	};
	// ----------------
	// ----------------
};
// **********************************
// **********************************
// KSpinBox Widget
class KSpinBox : public QSpinBox
{
	Q_OBJECT
public:
	// Destructeur
	~KSpinBox() {};
	// Constructeurs
	KSpinBox( QWidget *parent = nullptr )
		: QSpinBox( parent )
	{

		setMinimum( 0 );
		setMaximum(10000000);
		setSingleStep(1);
		setAlignment( Qt::AlignCenter );
		//setReadOnly( true );
	};
	// ----------------
	// Getters & Setters
	void setRect( int x, int y, int w, int h )
	{
		setGeometry( x, y, w, h );

	};
	// ----------------
	// ----------------
};
// **********************************
// **********************************
// KDOubleSpinBox Widget
class KDoubleSpinBox : public QDoubleSpinBox
{
	Q_OBJECT
private:
	// Initialisation du widget
	void init()
	{
		setMinimum( 0 );
		setMaximum(10000000);
		setSingleStep(1);
		setDecimals( 2 );
		setAlignment( Qt::AlignCenter );
		//setReadOnly( true );
	};
	// ----------------
	// ----------------
public:
	// Destructeur
	~KDoubleSpinBox() {};
	// Constructeurs
	KDoubleSpinBox( QWidget *parent = nullptr )
		: QDoubleSpinBox( parent )
	{
		init();
	};
	KDoubleSpinBox( double val, QWidget *parent = nullptr )
		: QDoubleSpinBox( parent )
	{
		init();
		setValue( val );
	};
	KDoubleSpinBox( double val, QString suffix, QWidget *parent = nullptr )
		: QDoubleSpinBox( parent )
	{
		init();
		setValue( val );
		setSuffix( " " + suffix );
	};
	// ----------------
	// Getters & Setters
	void setRect( int x, int y, int w, int h )
	{
		setGeometry( x, y, w, h );

	};
	void isLatitude( bool isTrue )
	{
		if ( isTrue )
		{
			setMinimum( -90 );
			setMaximum( 90 );
			setSingleStep( 0.01 );
			setDecimals( 6 );
			setWrapping( true );
		}
	};
	void isLongitude( bool isTrue )
	{
		if ( isTrue )
		{
			setMinimum( -180 );
			setMaximum( 180 );
			setSingleStep( 0.01 );
			setDecimals( 6 );
			setWrapping( true );
		}
	};
	// ----------------
	// ----------------
};
// **********************************
// **********************************
// KVBoxLayout Widget
class KVBoxLayout : public QVBoxLayout
{

};
// **********************************
// **********************************
// KHBoxLayout Widget
class KHBoxLayout : public QHBoxLayout
{

};
// **********************************
// **********************************
#endif // KIRKWIDGET_H
// **********************************
// **********************************
// **********************************
// **********************************
