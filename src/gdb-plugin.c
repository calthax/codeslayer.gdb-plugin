/*
 * Copyright (C) 2010 - Jeff Johnston
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <codeslayer/codeslayer.h>
#include <gtk/gtk.h>
#include <gmodule.h>
#include <glib.h>
#include "gdb-service.h"

G_MODULE_EXPORT void activate   (CodeSlayer *codeslayer);
G_MODULE_EXPORT void deactivate (CodeSlayer *codeslayer);

G_MODULE_EXPORT void
activate (CodeSlayer *codeslayer)
{
  gdb_start_debugger ();
  
  gdb_send_command ("-target-attach 15840\n");
  gdb_send_command ("break codeslayer_notebook_add_editor\n");
  gdb_send_command ("-exec-continue\n");
}

G_MODULE_EXPORT void 
deactivate (CodeSlayer *codeslayer)
{
  gdb_stop_debugger ();
}
