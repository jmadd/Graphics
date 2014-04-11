/*
 * mouse.c
 * -------
 * Mouse callbacks for the canvas.  Currently a little clunky, because
 * all rotations are in terms of the original axes of the model, rather
 * than the transformed axes of the current viewpoint.
 *
 * You shouldn't have to modify anything in this file.
 */

#include "common.h"

#include "mouse.h"
#include "data.h"
#include "drawing.h"
#include <stdio.h>

/* The frustum and zoom factor variables from canvas.c */
extern GLfloat fleft;
extern GLfloat fright;
extern GLfloat ftop;
extern GLfloat fbottom;
extern GLfloat zNear;
extern GLfloat zFar;
extern GLfloat zoomFactor;
bool add_points = true;

/* The current mode the mouse is in, based on what button(s) is pressed */
int mouse_mode;

/* The last position of the mouse since the last callback */
int m_last_x, m_last_y;



void myMouseButton(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && mode3d == false) {
		if (button == GLUT_LEFT_BUTTON) {
			// Add a point, if there is room
			if(x >= 195 && x <= 205) {
				x = 200;
			}
			if(num_i0_pts < MAX_POINT && add_points && x >= 200) {
				printf("Point %d added at x: %3d, y: %3d\n", num_i0_pts, x, y);
				printf("%d\n", num_i0_pts);
				if(num_i0_pts >= 29) add_points = false;
				else num_i0_pts++;
				i0_x[num_i0_pts] = x/5 - 40;
				i0_y[num_i0_pts] = (y/5 - 40) * -1;
			}
			else if (x < 200) {
				printf("Points must be entered to the right of the green line.\n");
			}
			else {
				printf("No more points can be added. You have exceeded the maximum.\n");
			}
		}
		if (button == GLUT_RIGHT_BUTTON) {
			if(num_i0_pts >= 0) {
				printf("Point %d deleted\n", num_i0_pts);
				i0_x[num_i0_pts] = 0;
				i0_y[num_i0_pts] = 0;
				num_i0_pts--;
			}
		}
	}

	else if (state == GLUT_DOWN && mode3d == true) {
		m_last_x = x;
		m_last_y = y;

		if (button == GLUT_LEFT_BUTTON) {
			mouse_mode = MOUSE_ROTATE_YX;
		} else if (button == GLUT_MIDDLE_BUTTON) {
			mouse_mode = MOUSE_ROTATE_YZ;
		} else if (button == GLUT_RIGHT_BUTTON) {
			mouse_mode = MOUSE_ZOOM;
		}
	}

	glutPostRedisplay();
}

void myMouseMotion(int x, int y) {
	double d_x, d_y;	/* The change in x and y since the last callback */

	d_x = x - m_last_x;
	d_y = y - m_last_y;

	m_last_x = x;
	m_last_y = y;
	if(mode3d==false)return;
	if (mouse_mode == MOUSE_ROTATE_YX) {
		/* scaling factors */
		d_x /= 2.0;
		d_y /= 2.0;

		glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
		glRotatef(-d_y, 1.0, 0.0, 0.0);	/* x-axis rotation */

	} else if (mouse_mode == MOUSE_ROTATE_YZ) {
		/* scaling factors */
		d_x /= 2.0;
		d_y /= 2.0;

		glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
		glRotatef(-d_y, 0.0, 0.0, 1.0);	/* z-axis rotation */

	} else if (mouse_mode == MOUSE_ZOOM) {
		d_y /= 100.0;

		zoomFactor += d_y;

		if (zoomFactor <= 0.0) {
			/* The zoom factor should be positive */
			zoomFactor = 0.001;
		}

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		/*
		 * glFrustum must receive positive values for the near and far
		 * clip planes ( arguments 5 and 6 ).
		 */
		glFrustum(fleft*zoomFactor, fright*zoomFactor,
			fbottom*zoomFactor, ftop*zoomFactor,
			-zNear, -zFar);
	}

	/* Redraw the screen */
	glutPostRedisplay();
}

/* end of mouse.c */

