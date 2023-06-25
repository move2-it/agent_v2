#include "JustJoinItDataFilter.hpp"

JustJoinItDataFilter::JustJoinItDataFilter()
{

}

Error_Code_T JustJoinItDataFilter::filter(QList<OffertData>& offertData)
{
    Error_Code_T result = Error_Code_T::SUCCESS;

    offertData.erase(std::remove_if(offertData.begin(), offertData.end(), [&](const OffertData& offert){
        bool eraseResult = false;

        if(offert.multilocation.length() == 1)
        {
            for(const auto& offertSearch : qAsConst(offertData))
            {
                for(const auto& location : qAsConst(offertSearch.multilocation))
                {
                    if((location.slugId == offert.idOffert) && (location.slugId != offertSearch.idOffert))
                    {
                        eraseResult = true;
                        break;
                    }
                }
                if(eraseResult == true)
                {
                    break;
                }
            }
        }

        return eraseResult;
    }), offertData.end());

    if(offertData.isEmpty() == true)
    {
        result = Error_Code_T::ZEROLENGTH;
    }

    return result;
}
