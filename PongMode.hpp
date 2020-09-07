#include "ColorTextureProgram.hpp"

#include "Mode.hpp"
#include "GL.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

/*
 * PongMode is a game mode that implements a single-player game of Pong.
 */

struct PongMode : Mode {
	PongMode();
	virtual ~PongMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	glm::vec2 court_radius = glm::vec2(7.0f, 7.0f);
	glm::vec2 paddle_radius = glm::vec2(0.2f, 1.0f);
	glm::vec2 paddle_radius_h = glm::vec2(1.0f, 0.2f);
	glm::vec2 ball_radius = glm::vec2(0.2f, 0.2f);

	glm::vec2 left_paddle = glm::vec2(-court_radius.x + 0.5f, 0.0f);
	glm::vec2 right_paddle = glm::vec2( court_radius.x - 0.5f, 0.0f);
	glm::vec2 top_paddle = glm::vec2(0.0f, -court_radius.y + 0.5f);
	glm::vec2 bottom_paddle = glm::vec2(0.0f, court_radius.y - 0.5f);

	glm::vec2 ball = glm::vec2(0.0f, 0.0f);
	glm::vec2 ball_velocity = glm::vec2(0.0f, 0.0f);
	glm::u8vec4 top_color = glm::u8vec4(0xd1, 0xbb, 0x54, 0xff);
	glm::u8vec4 bottom_color = glm::u8vec4(0xd1, 0xbb, 0x54, 0xff);
	glm::u8vec4 left_color = glm::u8vec4(0xd1, 0xbb, 0x54, 0xff);
	glm::u8vec4 right_color = glm::u8vec4(0xd1, 0xbb, 0x54, 0xff);

	// now kept in track 
	// uint32_t left_score = 0;
	// uint32_t right_score = 0;

	// the real score
	uint32_t player_score = 0;
	uint32_t ai_score = 0;

	int num_win = 1;
	bool draw_arrow = true;
	bool draw_ok = false;
	bool draw_not_ok = false;
	bool fallen_through = false;
	glm::u8vec4 fall_through = glm::u8vec4(0,0,0,0);
	// green color side
	// int green_color_side = -1;
	// float k = 1.0f;

	// 1 for player; 2 for AI; 0 for wall
	int last_bounce_id = -1;

	float ai_offset = 0.0f;
	float ai_offset_update = 0.0f;
	float reveal_checkpoint = 3.0f;
	float ball_dir = 0.0f;
	float timer = 0.0f;

	//----- pretty rainbow trails -----

	float trail_length = 1.3f;
	std::deque< glm::vec3 > ball_trail; //stores (x,y,age), oldest elements first

	//----- opengl assets / helpers ------

	//draw functions will work on vectors of vertices, defined as follows:
	struct Vertex {
		Vertex(glm::vec3 const &Position_, glm::u8vec4 const &Color_, glm::vec2 const &TexCoord_) :
			Position(Position_), Color(Color_), TexCoord(TexCoord_) { }
		glm::vec3 Position;
		glm::u8vec4 Color;
		glm::vec2 TexCoord;
	};
	static_assert(sizeof(Vertex) == 4*3 + 1*4 + 4*2, "PongMode::Vertex should be packed");

	//Shader program that draws transformed, vertices tinted with vertex colors:
	ColorTextureProgram color_texture_program;

	//Buffer used to hold vertex data during drawing:
	GLuint vertex_buffer = 0;

	//Vertex Array Object that maps buffer locations to color_texture_program attribute locations:
	GLuint vertex_buffer_for_color_texture_program = 0;

	//Solid white texture:
	GLuint white_tex = 0;

	//matrix that maps from clip coordinates to court-space coordinates:
	glm::mat3x2 clip_to_court = glm::mat3x2(1.0f);
	// computed in draw() as the inverse of OBJECT_TO_CLIP
	// (stored here so that the mouse handling code can use it to position the paddle)

};
