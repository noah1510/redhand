#include "redhand/event/drawing_event.hpp"
#include "redhand/game_object.hpp"
#include "redhand/texture.hpp"
#include <filesystem>
#include <glm/detail/type_vec.hpp>
#include <shared_mutex>

namespace redhand{
    class Actor : private redhand::game_object{
        friend class redhand::complex_world;
        private:

            /**
             * @brief The mutex for locking the image.
             * 
             */
            std::shared_mutex mutex_image;

            /**
             * @brief This is the image attached to this actor and its size will determine the width of the actor
             * 
             */
            std::shared_ptr<redhand::texture2D> image;

            /**
             * @brief This is the onLoop function just calls act and is needed by the complexWorld for the gameLoopEvent.
             * 
             * @param evt The game_loop_event containing a pointer to the engine and a time difference
             */
            void onLoop(game_loop_event evt) override;

            /**
             * @brief The mutex for locking the actor scale.
             * 
             */
            std::shared_mutex mutex_actorScale;

            /**
             * @brief This specifies the height of the actor.
             * If there is no texture attached to the width is also the width. 
             * Otherwise it is determined by the scale of the image attached to this actor and then scaled by this factor.
             * 
             */
            float actorScale = 1.0f;

        public:

            /**
             * @brief Construct a new Actor object with no image.
             * 
             */
            Actor();

            /**
             * @brief Construct a new Actor object with the supplied image.
             * 
             */
            Actor(redhand::texture2D*);

            /**
             * @brief The act method is called by the world to give actors a chance to perform some action.
             * 
             * @param evt This is the event, that holds a pointer to the engine and the time difference from the last call
             */
            virtual void act(game_loop_event evt);

            /**
             * @brief This returns the x position of the Actor in the world
             * 
             * @return float The x position
             */
            float getX();

            /**
             * @brief This returns the y position of the Actor in the world
             * 
             * @return float The y position
             */
            float getY();

            /**
             * @brief Set the Image of the Actor and automatically resizes accordingly
             * 
             * @param img The image that the Actor will now own
             */
            void setImage(redhand::texture2D* img);

            /**
             * @brief Set the Image of the Actor and automatically resizes accordingly
             * 
             * @param img The location of the image the actor will use now
             */
            void setImage(std::filesystem::path img);

            /**
             * @brief scale the actor by the given factor
             * 
             * @param scale the scale the actor shuld have now
             */
            void setActorScale(float scale);

            /**
             * @brief scale the Actor by the given factor
             * 
             * @param scale the factor the actor will be scaled with
             */
            void scaleActor(float scale = 1.0f);

            /**
             * @brief Get the scaling factor of the actor
             * 
             * @return float the scale of the actor
             */
            float getActorScale();

            /**
             * @brief Get the size of the Actor (width and height)
             * 
             * @return glm::vec2 the dimensions of the Actor
             */
            glm::vec2 getSize();
    };
}