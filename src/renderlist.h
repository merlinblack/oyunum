#ifndef __RENDERLIST_H
#define __RENDERLIST_H

#include <list>
#include <algorithm>
#include <memory>

// Renderable interface.
//

class Renderable
{
    int order;
    public:

    Renderable() : order(0)
    {}

    ~Renderable()
    {}

    virtual void render() = 0;

    inline void setOrder( int newOrder )
    {
        order = newOrder;
    }

    inline int getOrder() {
        return order;
    }

    // For sorting.
    inline bool operator< ( const Renderable& rhs ) const
    {
        return order < rhs.order;
    }
    
    // For removing.
    inline bool operator== ( const Renderable& rhs ) const
    {
        return this == &rhs;
    }
};

using RenderablePtr = std::shared_ptr<Renderable>;

class RenderList : public Renderable
{
    std::list<RenderablePtr> list;

    public:

    inline void sort()
    {
        list.sort(
                []( const RenderablePtr& lhs, const RenderablePtr& rhs ) { return lhs->getOrder() < rhs->getOrder(); }
                );
    }
    
    // Adds item according to it's sort order.
    inline void insert( RenderablePtr&& item )
    {
        list.emplace_back( std::move( item ) );
        sort();
    }

    inline void remove( const RenderablePtr& item )
    {
        list.remove( item );
    }

    // Adds item at the end and adjusts it's sort order
    inline void add( RenderablePtr&& item )
    {
        if( list.empty() ) {
            insert( std::forward<RenderablePtr>(item) );
            return;
        }

        int maxOrder = list.back()->getOrder();

        maxOrder++;

        item->setOrder( maxOrder );

        list.emplace_back( std::move( item ) );
    }

    inline void render()
    {
        for( auto& item : list ) 
        {
            item->render();
        }
    }
};

using RenderListPtr = std::shared_ptr<RenderList>;

#endif // __RENDERLIST_H
