/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2007-04-21
 * Description : Structures to define Albums used in CoreDb
 *
 * Copyright (C) 2007-2011 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
 * Copyright (C) 2005      by Renchi Raju <renchi dot raju at gmail dot com>
 * Copyright (C) 2006-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef DIGIKAM_CORE_DB_ALBUM_INFO_H
#define DIGIKAM_CORE_DB_ALBUM_INFO_H

// Qt includes

#include <QString>
#include <QList>
#include <QDateTime>

// Local includes

#include "coredbconstants.h"

namespace Digikam
{

typedef QPair<int, int> YearMonth;

/**
 * \class AlbumRootInfo
 */
class AlbumRootInfo
{
public:

    explicit AlbumRootInfo()
      : id    (0),
        type  (AlbumRoot::UndefinedType),
        status(0)
    {
    };

public:

    int             id;
    QString         label;
    AlbumRoot::Type type;
    int             status;
    QString         identifier;
    QString         specificPath;
};

// --------------------------------------------------------------------------

/**
 * \class AlbumInfo
 * A container class for transporting album information
 * from the database to AlbumManager
 */
class AlbumInfo
{
public:

    typedef QList<AlbumInfo> List;

public:

    explicit AlbumInfo()
      : id         (0),
        albumRootId(0),
        iconId     (0)
    {
    };

    bool isNull() const
    {
        return (id == 0);
    }

    /**
     * needed for sorting
     */
    bool operator<(const AlbumInfo& info) const
    {
        // include album root id?

        return (relativePath < info.relativePath);
    }

public:

    int       id;
    int       albumRootId;
    QString   relativePath;
    QString   caption;
    QString   category;
    QDate     date;
    qlonglong iconId;
};

// --------------------------------------------------------------------------

/**
 * \class TagInfo
 * A container class for transporting tag information
 * from the database to AlbumManager
 */
class TagInfo
{
public:

    typedef QList<TagInfo> List;

public:

    explicit TagInfo()
      : id    (0),
        pid   (0),
        iconId(0)
    {
    };

    bool isNull() const
    {
        return (id == 0);
    }

    bool operator<(const TagInfo& info) const
    {
        return (name < info.name);
    }

public:

    int       id;
    int       pid;
    QString   name;
    QString   icon;
    qlonglong iconId;
};

// --------------------------------------------------------------------------

/**
 * \class SearchInfo
 * A container class for transporting search information
 * from the database to AlbumManager
 */
class SearchInfo
{
public:

    typedef QList<SearchInfo> List;

public:

    explicit SearchInfo()
      : id  (0),
        type(DatabaseSearch::UndefinedType)
    {
    };

    bool isNull() const
    {
        return (id == 0);
    }

    /**
     * needed for sorting
     */
    bool operator<(const SearchInfo& info) const
    {
        return (id < info.id);
    }

public:

    int                  id;
    QString              name;
    DatabaseSearch::Type type;
    QString              query;
};

// --------------------------------------------------------------------------

class AlbumShortInfo
{
public:

    explicit AlbumShortInfo()
      : id         (0),
        albumRootId(0)
    {
    };

    bool isNull() const
    {
        return (id == 0);
    }

public:

    int     id;
    QString relativePath;
    int     albumRootId;
};

// --------------------------------------------------------------------------

class TagShortInfo
{
public:

    explicit TagShortInfo()
      : id (0),
        pid(0)
    {
    };

    bool isNull() const
    {
        return (id == 0);
    }

public:

    int     id;
    int     pid;
    QString name;
};

// --------------------------------------------------------------------------

class ItemShortInfo
{
public:

    explicit ItemShortInfo()
      : id         (0),
        albumID    (0),
        albumRootID(0)
    {
    };

    bool isNull() const
    {
        return (id == 0);
    }

public:

    qlonglong id;
    QString   itemName;
    int       albumID;
    int       albumRootID;
    QString   album;
};

// --------------------------------------------------------------------------

class ItemScanInfo
{
public:

    explicit ItemScanInfo()
      : id      (0),
        albumID (0),
        status  (DatabaseItem::UndefinedStatus),
        category(DatabaseItem::UndefinedCategory),
        fileSize(0)
    {
    };

    bool isNull() const
    {
        return (id == 0);
    }

public:

    qlonglong              id;
    int                    albumID;
    QString                itemName;
    DatabaseItem::Status   status;
    DatabaseItem::Category category;
    QDateTime              modificationDate;
    qlonglong              fileSize;
    QString                uniqueHash;
};

// --------------------------------------------------------------------------

class CommentInfo
{
public:

    explicit CommentInfo()
      : id     (-1),
        imageId(-1),
        type   (DatabaseComment::UndefinedType)
    {
    };

    bool isNull() const
    {
        return (id == -1);
    }

public:

    int                   id;
    qlonglong             imageId;
    DatabaseComment::Type type;
    QString               author;
    QString               language;
    QDateTime             date;
    QString               comment;
};

// --------------------------------------------------------------------------

class CopyrightInfo
{
public:

    explicit CopyrightInfo()
      : id(-1)
    {
    };

    bool isNull() const
    {
        return (id == -1);
    }

public:

    qlonglong id;
    QString   property;
    QString   value;
    QString   extraValue;
};

// --------------------------------------------------------------------------

class ImageHistoryEntry
{
public:

    explicit ImageHistoryEntry()
      : imageId(0)
    {
    };

    bool isNull() const
    {
        return (imageId == 0);
    }

public:

    qlonglong imageId;
    QString   uuid;
    QString   history;
};

// --------------------------------------------------------------------------

class ImageRelation
{
public:

    explicit ImageRelation()
      : subjectId(0),
        objectId (0),
        type     (DatabaseRelation::UndefinedType)
    {
    }

public:

    qlonglong              subjectId;
    qlonglong              objectId;
    DatabaseRelation::Type type;
};

// --------------------------------------------------------------------------

class TagProperty
{
public:

    explicit TagProperty()
      : tagId(-1)
    {
    };

    bool isNull() const
    {
        return (tagId == -1);
    }

public:

    int     tagId;
    QString property;
    QString value;
};

// --------------------------------------------------------------------------

class ImageTagProperty
{
public:

    explicit ImageTagProperty()
      : imageId(-1),
        tagId  (-1)
    {
    };

    bool isNull() const
    {
        return (imageId == -1);
    }

public:

    qlonglong imageId;
    int       tagId;
    QString   property;
    QString   value;
};

} // namespace Digikam

#endif // DIGIKAM_CORE_DB_ALBUM_INFO_H
