#include "Test.h"

// A vertical stack of boxes on a floor. It should settle, fall asleep (gray),
// and wake up when something is dropped on it.
class BoxStack : public Test
{
  public:
    BoxStack()
    {
        const float screenWidth  = g_draw.screenWidth;
        const float screenHeight = g_draw.screenHeight;

        // Floor at the bottom of the screen (screen y grows downward)
        m_world->CreateBody( d2BoxShape( screenWidth, 20.0f ), { screenWidth / 2.0f, screenHeight - 10.0f }, 0.0f );

        constexpr int   stackSize = 8;
        constexpr float boxSize   = 40.0f;
        const float     floorTop  = screenHeight - 20.0f;
        for( int i = 0; i < stackSize; ++i )
            {
                m_world->CreateBody( d2BoxShape( boxSize, boxSize ),
                                     { screenWidth / 2.0f, floorTop - boxSize / 2.0f - i * ( boxSize + 2.0f ) },
                                     1.0f );
            }
    }

    void
    Input() override
    {
        // Drop a box on the (possibly sleeping) stack
        if( IsMouseButtonPressed( MOUSE_LEFT_BUTTON ) )
            {
                auto mousePos = GetMousePosition();
                m_world->CreateBody( d2BoxShape( 40.0f, 40.0f ), { mousePos.x, mousePos.y }, 1.0f );
            }
    }

    static Test *
    Create()
    {
        return new BoxStack;
    }
};

static int testIndex = RegisterTest( "Stability", "Box Stack", BoxStack::Create );
