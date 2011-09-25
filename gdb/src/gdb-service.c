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
 
#include <gtk/gtk.h>
#include "gdb-service.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static GIOChannel *channel_read;
static GIOChannel *channel_write;


static gboolean iochannel_read (GIOChannel   *channel, 
                                GIOCondition  condition);

void 
gdb_start_debugger ()
{
  int	stdin_pipe[2];
  int	stdout_pipe[2];

  int pid;

  if (pipe (stdin_pipe) != 0	|| pipe (stdout_pipe) != 0)
    {
      g_warning ("Not able to create the pipes.");  
      return;
    }

  pid = fork ();

  if (pid == -1)
    {
      g_warning ("Not able to fork the process.");
      return;
    }

  if (pid == 0)
    {
      close (0);
      dup (stdin_pipe[0]);
      close (stdin_pipe[0]); 
      close (stdin_pipe[1]); 

      close (1);
      dup (stdout_pipe[1]);
      close (stdout_pipe[0]);
      close (stdout_pipe[1]);

      execlp ("gdb", "gdb", "--interpreter=mi", (char *)NULL);

      g_warning ("Not able to communicate with gdb.");
    }
  else
    {
      channel_read = g_io_channel_unix_new (stdout_pipe[0]);
      channel_write = g_io_channel_unix_new (stdin_pipe[1]);
      
      close (stdin_pipe[0]);
      close (stdout_pipe[1]);

      if (channel_read == NULL || channel_write == NULL)
        g_error ("The channels could not be created.");
        
      g_io_add_watch (channel_read, G_IO_IN | G_IO_HUP, (GIOFunc) iochannel_read, NULL);        
    }
}

void 
gdb_stop_debugger ()
{
}

void 
gdb_send_command (char *cmd)
{
  GIOStatus ret_value;
  gsize length;

  g_print ("%s\n", cmd);
  
  ret_value = g_io_channel_write_chars (channel_write, cmd, -1, &length, NULL);
  if (ret_value == G_IO_STATUS_ERROR)
    g_error ("The changes could not be written to the pipe.");
  else
    g_io_channel_flush (channel_write, NULL);
}

static gboolean
iochannel_read (GIOChannel *channel, 
                GIOCondition condition)
{
  GIOStatus ret_value;
  gchar *message;
  gsize length;

  if (condition == G_IO_HUP)
    g_error ("The pipe has died.");

  ret_value = g_io_channel_read_line (channel_read, &message, &length, NULL, NULL);
  if (ret_value == G_IO_STATUS_ERROR)
    g_error ("Could not read from the pipe.");

  g_print ("%s", message);

  return TRUE;
}                
