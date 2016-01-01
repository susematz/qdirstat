/*
 *   File name: DataColumns.h
 *   Summary:	Data column mapping
 *   License:	GPL V2 - See file LICENSE for details.
 *
 *   Author:	Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 */


#ifndef DataColumns_h
#define DataColumns_h

#include <QList>
#include <QString>
#include <QObject>
#include <QTextStream>


namespace QDirStat
{
    /**
     * Data columns for data model, view and sorting.
     **/
    typedef enum
    {
	NameCol	       = 0,	// File / directory name
	PercentBarCol,		// Graphical percentage bar
	PercentNumCol,		// Numeric percentage Value
	TotalSizeCol,		// Total subtree size
	OwnSizeCol,		// Own size of file / directory
	TotalItemsCol,		// Total number of items    in subtree
	TotalFilesCol,		// Total number of files    in subtree
	TotalSubDirsCol,	// Total number of subdirs  in subtree
	LatestMTimeCol,		// Latest modification time in subtree
	ReadJobsCol,		// Number of pending read jobs in subtree
	UndefinedCol = -99
    } DataColumn;

    typedef QList<DataColumn> DataColumnList;


    /**
     * Singleton class for data columns.
     *
     * This class keeps track what columns should be displayed and in what
     * order and how to map view columns to model columns and vice versa.
     *
     * The model uses the DataColumn enum internally. For the view, however,
     * the columns may be rearranged, and any column may be ommitted (not
     * displayed at all). This class handles that mapping.
     **/
    class DataColumns: public QObject
    {
	Q_OBJECT

    public:
	/**
	 * Return the singleton instance for this class. This will create the
	 * singleton upon the first call.
	 **/
	static DataColumns * instance();

	/**
	 * Map a view column to the corresponding model column.
	 * (static version)
	 **/
	static DataColumn fromViewCol( int viewCol )
	    { return instance()->mappedCol( static_cast<DataColumn>( viewCol ) ); }

	/**
	 * Map a model column to the corresponding view column.
	 * (static version)
	 **/
	static DataColumn toViewCol( int modelCol )
	    { return instance()->reverseMappedCol( static_cast<DataColumn>( modelCol ) ); }

	/**
	 * Map a view column to the corresponding model column.
	 **/
	DataColumn mappedCol( DataColumn viewCol ) const;

	/**
	 * Map a model column to the corresponding view column.
	 **/
	DataColumn reverseMappedCol( DataColumn modelCol ) const;


	/**
	 * Set the column order and what columns to display.
	 *
	 * Example:
	 *
	 *   DataColumnList col;
	 *   col << QDirStat::NameCol,
	 *	 << QDirStat::PercentBarCol,
	 *	 << QDirStat::PercentNumCol,
	 *	 << QDirStat::TotalSizeCol;
	 *   DataColumns->instance()->setColumns( col );
	 *
	 * NOTICE: If a data view is active, use DirTreeModel::setColumns()
	 * instead (which will call this function in turn) so the view is
	 * correctly notified about this change.
	 *
	 * This will emit a columnsChanged() signal.
	 */
	void setColumns( const DataColumnList & columns );

	/**
	 * Return the model colums that are currently being displayed.
	 **/
	const DataColumnList & columns() const { return _columns; }

	/**
	 * Return the number of columns that are curently displayed.
	 **/
	int colCount() const { return _columns.size(); }

        /**
         * Return a string representation of a column.
         **/
        static QString toString( DataColumn col );

    signals:
	/**
	 * Emitted when the columns changed (after setColumns() ).
	 **/
	void columnsChanged();


    protected:
	/**
	 * Constructor. This is not meant for general use; use
	 * DataColumns::instance() and the static methods instead.
	 **/
	DataColumns();


	// Data members

	static DataColumns * _instance;
	DataColumnList	     _columns;

    };	// class DataColumns


    /**
     * Print a DataColumn in text form to a debug stream.
     **/
    inline QTextStream & operator<< ( QTextStream & stream, DataColumn col )
    {
        stream << DataColumns::toString( col );

	return stream;
    }


}	// namespace QDirStat

#endif	// DataColumns_h
