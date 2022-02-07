/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2006-09-15
 * Description : Exiv2 library interface.
 *               Shared data container.
 *
 * Copyright (C) 2006-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2006-2013 by Marcel Wiesweg <marcel dot wiesweg at gmx dot de>
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

#ifndef META_ENGINE_DATA_H
#define META_ENGINE_DATA_H

// Qt includes

#include <QExplicitlySharedDataPointer>

// Local includes

#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT MetaEngineData
{
public:

    MetaEngineData();
    MetaEngineData(const MetaEngineData&);
    ~MetaEngineData();

    MetaEngineData& operator=(const MetaEngineData&);

public:

    // Declared as public due to use in MetaEngine::Private class
    class Private;

private:

    QExplicitlySharedDataPointer<Private> d;

    friend class MetaEngine;
};

} // namespace Digikam

#endif // META_ENGINE_DATA_H
