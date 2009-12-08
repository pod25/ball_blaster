/*
 * File: physics.cpp
 */
#include "common.h"

void physics::calculate_ball_acceleration() {
	ball_acc = lev.get_gravity();
}

void physics::apply_ball_acceleration(double dt, double amount) {
	lev.set_ball_vel(lev.get_ball_vel() + amount * dt * ball_acc);
}

void physics::step(double dt, uint num_calls_left) {
	if (!num_calls_left) {
		lev.set_ball_vel(vec(0, 0)); // Stop ball, it have probably got stuck
		return;
	}
	vec v  = lev.get_ball_vel();
	vec p1 = lev.get_ball_pos();
	vec p2 = p1 + dt*v;

	lev.set_ball_pos(p2);
}

void physics::init_level_simulation() {
	lev.set_ball_pos((vec(lev.cannon_coords()) + vec(0.5, 0.5))*lev.get_square_scale());
	//lev.set_ball_vel(vec(lev.get_cannon()->_shot_vec)/100);
	lev.set_ball_vel(vec(0,0));
	calculate_ball_acceleration();
}

void physics::step(double dt) {
	apply_ball_acceleration(dt, .5);
	step(dt, MAX_FRAME_ITERATIONS);
	apply_ball_acceleration(dt, .5);
}

/*

    
    (define move-ball-and-platform (let ((max-calls 10))
    (lambda (bp1 bp2 pp1 pp2 b-vel p-vel call-nr) ;;Ball posiotion 1 and 2, platform position 1 and 2, ball velocity, platform velocity
    (let* (
            (current-level level) ;;The level currently being updated
            (bdp (vector-difference bp2 bp1)) ;;Ball delta position, the vector between bp1 and bp2
            (pdp (vector-difference pp2 pp1)) ;;Platform delta position, the vector between pp1 and pp2
            (c-rad (circle-radius (ball-circle (level-ball level)))) ;;The radius of the circle (the ball's radius)
            (block-instances (level-block-instances level)) ;;The level block instances
            (platform (level-platform level)) ;;The level platform
            (best-amount-of-distance-to-block 1   ) ;;Contains the time of hitting the object
            (best-nnormal-to-bounce-tangent   NULL) ;;The normalized normal of the bounce surface (if bounce occurs)
            (best-block-instance-bounced-idx  NULL) ;;The object being bounced (if bounce occurs)
          )
      
        (define (off-set-ball) ;;In case of hit with moving object (in this case the platform)
            (point-imitate bp1 (vector-difference bp1 pp1))
            (point-imitate bp2 (vector-difference bp2 pp2))
            (set! bdp (vector-difference bp2 bp1))
            (point-imitate b-vel (vector-difference b-vel p-vel)) )
        
        (define (off-set-ball-back) ;;In case of hit with moving object (in this case the platform)
            (point-imitate bp1 (vector-sum bp1 pp1))
            (point-imitate bp2 (vector-sum bp2 pp2))
            (set! bdp (vector-difference bp2 bp1))
            (point-imitate b-vel (vector-sum b-vel p-vel)) )
        
        (define (bounce-ball) ;;Calculate the new position and velocity of the ball
            (when (number? best-block-instance-bounced-idx) ;; -1 for the platform
                (if (= best-block-instance-bounced-idx -1)
                    (begin
                        (off-set-ball)
                        ((game-engine 'platform-hit)))
                    ((game-engine 'block-instance-hit) best-block-instance-bounced-idx)))
            
            (point-imitate b-vel (damped-reflection b-vel (make-point 0 0) best-nnormal-to-bounce-tangent bounce-coefficient))
            (point-imitate bp1 (vector-sum bp1 (factor-of-vector bdp best-amount-of-distance-to-block)))
            (point-imitate bp2 (damped-reflection bp2 bp1 best-nnormal-to-bounce-tangent bounce-coefficient))
            (point-imitate pp1 (vector-sum pp1 (factor-of-vector pdp best-amount-of-distance-to-block)))
          
            (when (and (number? best-block-instance-bounced-idx) (= best-block-instance-bounced-idx -1)) (off-set-ball-back)) )
        
        ;;time for hit, point for hitting, normalized normal of the bounce surface, object bounceable or only for bounding
        (define (validate-hit-event amount-of-dist b-point b-nnormal real-object? block-inst-idx)
            (cond
                ((>= amount-of-dist best-amount-of-distance-to-block) #f) ;;Found bounce event that will occur earlier than this
                ((and (not real-object?) ;;Okay, just a bounding circle. Then the normal will point out from the circle.
                      (< (dot-product (vector-difference bp1 b-point) b-nnormal) c-rad)) #t) ;;distance d to tangent < ball radius (d may be negative)
                          ;;If this unequality is true, the circle is partly inside the bounding circle.
                (   (>= (* (dot-product (vector-difference bp1 b-point) b-nnormal)  ;; distance to tangent (may be negative)
                           (dot-product b-vel                           b-nnormal)) ;; velocity from tangent
                    0) #f) ;;The ball is not moving against the tangent
                ;((and (< amount-of-dist 0) real-object?) (begin (log-output "hitting at " amount-of-dist " ignored") #f))
                (else
                    (when real-object? ;;Update information about the bounce
                        (set! best-amount-of-distance-to-block amount-of-dist)
                        (set! best-nnormal-to-bounce-tangent   b-nnormal     )
                        (set! best-block-instance-bounced-idx  block-inst-idx))
                    #t)) )
        
        ;;b, c, root internal variables for solving the second order equation
        ;;t1 and t2 times for hitting/leaving the circle
        ;;Ball's position when hitting, normalized normal of the bounce surface
        (define hittest-circle                      (let ((b NULL) (c NULL) (root NULL) (t1 NULL) (t2 NULL) (bhp NULL) (hnn NULL))
        (lambda (p rad real-object? block-inst-idx) (let ((a (vec-square-length bdp)) (r (sqr (+ c-rad rad))))
            (cond
                ((>= (sqr (determinant bdp (vector-difference p bp1))) (* r a))
                    #f) ;;p is to far from the movement, or the ball isn't moving.
                ((begin (set! c (square-dist p bp1))
                        (set! b (* (+ (* (- (point-x bp2) (point-x bp1)) (- (point-x bp1) (point-x p)))
                                      (* (- (point-y bp2) (point-y bp1)) (- (point-y bp1) (point-y p)))) 2))
                        (set! root (+ (/ (- r c) a) (sqr (/ b (* 2 a))))) ;;sqrt((r-c)/a + b^2/4a^2)
                    (< root 0))
                    #f)
                ((begin
                        (set! root (sqrt root))
                        (set! t1 (- 0 root (/ b (* 2 a))))
                        (set! t2 (-   root (/ b (* 2 a))))
                    (or (> t1 1) (< t2 0) (and real-object? (< t2 (- t1)))) )
                    #f) ;;Either the ball is half way out of the circle or isn't about to enter it in this frame.
                ;((and (< t1 0) real-object?)
                ;    (begin (log-output "Circle-ht: " (print-convert p) " at negative time " t1 " " t2) #f))
                (else (begin ;(when (and (< t1 0) real-object?) (log-output "Circle-ht: " (print-convert p) " at negative time " t1 t2))
                        (set! bhp (vector-sum bp1 (factor-of-vector bdp t1))) ;;Ball hit point (the ball's position)
                        (set! hnn (normalized-vector (vector-difference bhp p))) ;;normalized normal at hit spot
                        (validate-hit-event t1 (vector-difference bhp (factor-of-vector hnn c-rad)) hnn real-object? block-inst-idx)))) ))))
      
        (define (hittest-corner p real-object? block-inst)
            (hittest-circle p 0 real-object? block-inst) )
      
        ;;Time for hiting the line, ball's position when hitting, bounce point, p1-p2 ration for bounce
        (define hittest-line (let ((t NULL) (bhp NULL) (bhp-scal-dp NULL) (bp NULL) (p1p2-ratio NULL))
        (lambda (p1 p2 real-object? block-inst-idx)
        (let* ( (dp (vector-difference p2 p1)) ;;Delta points, the vector from p1 to p2
                (nn (normalized-vector (turned-in-right-angle dp))) ;;The normalized normal to the line
                (bdp-scal-nn (dot-product bdp nn)) ) ;;The vector between ball pos 1 and ball pos 2, scalar with the normalized normal
            (cond
                ((= bdp-scal-nn 0) #f) ;;The ball is moving along the line
                ((begin (set! t (/ (dot-product (vector-difference p1 bp1) nn) bdp-scal-nn)) ;;time until center of the ball will hit the line
                    (< t 0)) ;;The center of the ball has already passed the line when doing this check
                    #f)
                ((begin (set! t (- t (abs (/ c-rad bdp-scal-nn)))) ;;time until the ball will hit the line
                    (>= t best-amount-of-distance-to-block))
                    #f)
                ((begin (set! p1p2-ratio (/ (dot-product (vector-difference bp1 p1) dp) (vec-square-length dp))) ;;Ball's pos in ratio between p1 and p2
                    (and (or (< p1p2-ratio 0) (> p1p2-ratio 1)) (< t 0))) ;;The ball isn't beside the line and isn't going to
                    #f)
                ((begin (set! bhp (vector-sum bp1 (factor-of-vector bdp t))) ;;Ball hit point (the ball's position)
                        (set! p1p2-ratio (/ (dot-product (vector-difference bhp p1) dp) (vec-square-length dp))) ;;The hit pos in ratio between p1 and p2
                    (or (< p1p2-ratio 0) (> p1p2-ratio 1)))
                    #f)
                (else (begin
                    (set! bhp-scal-dp (dot-product bhp dp))
                    (set! bp (vector-sum p1 (factor-of-vector dp p1p2-ratio)))
                    ;(when (and (< t 0) real-object?) (log-output "Line-ht: negative time " t))
                    (validate-hit-event t bp nn real-object? block-inst-idx)))) ))))
      
        (define (hittest-pol-instance pol-inst block-inst-idx)
        (let* ((points (polygon-points (pol-instance-pol pol-inst))) (numpoints (vector-length points)))
            (define (hit-test-sides-and-coorners idx prev-idx)
                (when (< idx numpoints)
                    (hittest-line (vector-ref points idx) (vector-ref points prev-idx) #t block-inst-idx)
                    (hittest-corner (vector-ref points idx) #t block-inst-idx)
                    (hit-test-sides-and-coorners (+ idx 1) idx)) )
            (when (hittest-circle (pol-instance-mc pol-inst) (pol-instance-radius pol-inst) #f block-inst-idx)
                (hit-test-sides-and-coorners 0 (- numpoints 1))) ))
        
        (define (hittest-block-instance block-inst block-inst-idx)
        (let* ((pol-instances (block-pol-instances (block-instance-block block-inst))) (num-pol-instances (vector-length pol-instances)))
            (define (hittest-pol-instances idx)
                (when (< idx num-pol-instances)
                    (hittest-pol-instance (vector-ref pol-instances idx) block-inst-idx)
                    (hittest-pol-instances (+ idx 1))) )
            (when (hittest-circle (block-instance-mc block-inst) (block-instance-radius block-inst) #f block-inst-idx)
                (hittest-pol-instances 0)) ))
        
        (define (hittest-all-block-instances)
        (let* ((num-block-instances (vector-length block-instances)))
            (define (hittest-block-instances idx)
                (when (< idx num-block-instances)
                    (hittest-block-instance (vector-ref block-instances idx) idx)
                    (hittest-block-instances (+ idx 1))) )
            (hittest-block-instances 0) ))
        
        (when (not (point-eq? bp1 bp2))
            (hittest-all-block-instances)
            ;;Hit-test the three walls
            (hittest-line (make-point 0 0) (make-point screen-width 0) #t NULL) ;;Top wall
            (hittest-line (make-point 0 0) (make-point 0 screen-height) #t NULL) ;;Left wall
            (hittest-line (make-point screen-width 0) (make-point screen-width screen-height) #t NULL) ;;Right wall
            ;(hittest-line (make-point 0 screen-height) (make-point screen-width screen-height) #t NULL) ;;Bottom "wall"
        )
        (off-set-ball)
        (hittest-block-instance platform -1) ;;-1 since the platform has no id
        (off-set-ball-back)
        (when (not (null? best-nnormal-to-bounce-tangent)) ;;A bounce occurdes
            (bounce-ball)
            (when (eq? current-level level)
                (if (< call-nr max-calls)
                    (move-ball-and-platform bp1 bp2 pp1 pp2 b-vel p-vel (+ call-nr 1))
                    (begin
                        (point-imitate bp2 bp1)
                        (point-imitate b-vel (make-point 0 0))
                        (point-imitate pp2 pp1)
                        (point-imitate p-vel (make-point 0 0)) ))))
    ))))
    
    (define step
    (let* ( (last-mouse-pos NULL) (mdp (make-point 0 0))) ;;mdp = mouse delta pos
    (lambda (ts) ;;time step in seconds
    (let* (
            (ball (level-ball level)) ;;level ball
            (b-vel (ball-vel ball)) ;;Velocity of the ball
            (c-mid (circle-mid (ball-circle ball))) ;;Midpoint of the circle (the ball)
            (platform (level-platform level)) ;;level platform
            (bp1 NULL) (bp2 NULL) ;;Ball position 1 (before step), ball position 2 (after step)
            (pp1 NULL) (pp2 NULL) ;;Platform position 1 (before step), platform position 2 (after step)
         )
    (when (> ts 0) ;;Critical since the platform may move when the ball movement is otherwise canseled
        (when gravity-on?
            (move-point b-vel 0 (* .5 gravity ts)))
    
        (set! bp1 c-mid)
        (set! bp2 (vector-sum c-mid (factor-of-vector b-vel ts)))
    
        (when (userinput-mouseinwindow userinput)
            (when (not (null? last-mouse-pos))
                (set! mdp (vector-difference (make-point (userinput-mousex userinput) (userinput-mousey userinput)) last-mouse-pos)))
            (set! last-mouse-pos (make-point (userinput-mousex userinput) (userinput-mousey userinput))))
      
        (set! pp1 (make-point 0 0))
        (set! pp2 (if (not (null? last-mouse-pos))
                      (vector-difference last-mouse-pos (block-instance-mc platform))
                      (make-point 0 0)))
        (set! pp2 (factor-of-vector pp2 (min (* ts 20) 1)))
      
        (if (< (+ (point-x (block-instance-brmin platform)) (point-x pp2)) 0)
            (set-point-x! pp2 (- 0 (point-x (block-instance-brmin platform))))
            (when (> (+ (point-x (block-instance-brmax platform)) (point-x pp2)) screen-width)
                (set-point-x! pp2 (- screen-width (point-x (block-instance-brmax platform))))))
        (if (< (+ (point-y (block-instance-brmin platform)) (point-y pp2)) (level-platform-border level))
            (set-point-y! pp2 (- (level-platform-border level) (point-y (block-instance-brmin platform))))
            (when (> (+ (point-y (block-instance-brmax platform)) (point-y pp2)) screen-height)
                (set-point-y! pp2 (- screen-height (point-y (block-instance-brmax platform))))))
    
        (move-ball-and-platform bp1 bp2 pp1 pp2 b-vel (factor-of-vector (vector-difference pp2 pp1) (/ 1 ts)) 1)
        (point-imitate c-mid bp2)
        (move-block-instance platform (point-x pp2) (point-y pp2))
    
        (when gravity-on?
            (move-point (ball-vel ball) 0 (* .5 gravity ts)))
    )))))
    
    (define (get-gravity-on?)
        gravity-on? )
  
    (define (set-gravity-on?! on?)
        (set! gravity-on? on?) )
  
    (define (switch-gravity-on?!)
        (set! gravity-on? (not gravity-on?)) )
    
    (define (get-bounce-coefficient)
        bounce-coefficient )
  
    (define (set-bounce-coefficient! val)
        (set! bounce-coefficient (max val 0)) )
    
    (define (get-damping)
        (- 1 (get-bounce-coefficient)) )
  
    (define (set-damping! val)
        (set-bounce-coefficient! (- 1 val)) )
    
    (define (physics-dispatch query . args)
        (cond
            ((not (symbol? query)) (err query " is not a symbol"))
            
            ((eq? query 'step                   )    step                   )
            ;((eq? query    'get-gravity-on?     )       get-gravity-on?     )
            ((eq? query    'set-gravity-on?!    )       set-gravity-on?!    )
            ((eq? query 'switch-gravity-on?!    )    switch-gravity-on?!    )
            ;((eq? query 'get-bounce-coefficient )    get-bounce-coefficient )
            ;((eq? query 'set-bounce-coefficient!)    set-bounce-coefficient!)
            ;((eq? query 'get-bounce-damping     )    get-bounce-damping     )
            ;((eq? query 'set-bounce-damping!    )    set-bounce-damping!    )
            
            (else                                 (err "invalid query to physics-engine: " query))) )
    
    physics-dispatch ))
*/

//