#ifndef ACTIONTYPES_H
#define ACTIONTYPES_H

#include <QString>

namespace ActionTypes{
    const QString TEST = "TEST";
    namespace FncTab{
        const QString CHANGE_CURRENT_FUNCTION = "CHANGE_CURRENT_FUNCTION";
        const QString CALL_CURRENT_FUNCTION = "CALL_CURRENT_FUNCTION";
    }
    namespace TrTab {
        const QString CHANGE_CURRENT_TR = "CHANGE_CURRENT_TR";
        const QString CALL_CURRENT_TR = "CALL_CURRENT_TR";
    }
    namespace AssetTab {
        const QString REQUEST_ASSET_LIST = "REQUEST_ASSET_LIST";
    }
}
#endif // ACTIONTYPES_H
