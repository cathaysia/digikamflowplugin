/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2010-12-15
 * Description : white balance color correction settings container
 *
 * Copyright (C) 2007-2021 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2008      by Guillaume Castagnino <casta at xwing dot info>
 * Copyright (C) 2010      by Martin Klapetek <martin dot klapetek at gmail dot com>
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

#ifndef DIGIKAM_WB_CONTAINER_H
#define DIGIKAM_WB_CONTAINER_H

// Qt includes

#include <QString>

// Local includes

#include "digikam_export.h"

namespace Digikam
{

class FilterAction;

class DIGIKAM_EXPORT WBContainer
{

public:

    WBContainer();

    bool   isDefault()                                            const;
    bool   operator==(const WBContainer& other)                   const;

    void   writeToFilterAction(FilterAction& action,
                               const QString& prefix = QString()) const;

    static WBContainer fromFilterAction(const FilterAction& action,
                                        const QString& prefix = QString());

public:

    double black;
    double expositionMain;
    double expositionFine;
    double temperature;
    double green;
    double dark;
    double gamma;
    double saturation;
};

} // namespace Digikam

#endif // DIGIKAM_WB_CONTAINER_H
