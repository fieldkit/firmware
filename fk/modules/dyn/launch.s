/**
 * This software is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this source code. If not, see <http://www.gnu.org/licenses/>.
 */
.file   "launch.s"
.syntax unified
.thumb

.section ".text"
.align   2

	.thumb_func
	.type   fk_dyn_run, %function
	.global fk_dyn_run
fk_dyn_run:
	.fnstart
	.cantunwind

	/* Assign r9 to GOT location. */
	mov     r9, r0

	/* Push GOT value onto stack, the application can then use
	 * this to check for corruption. */
	push    {r9, lr}
	blx     r1
	pop     {r9, pc}

	.fnend
	.size   fk_dyn_run, .-fk_dyn_run


.end
