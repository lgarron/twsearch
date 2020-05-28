
function unimplemented(f: string): () => void {
  return () => {throw new Error(`Unimplemented: ${f}`)};
};

export const stubWasi = {
  clock_time_get: unimplemented("clock_time_get"),
  environ_get: unimplemented("environ_get"),
  environ_sizes_get: unimplemented("environ_sizes_get"),
  fd_close: unimplemented("fd_close"),
  fd_fdstat_get: unimplemented("fd_fdstat_get"),
  fd_fdstat_set_flags: unimplemented("fd_fdstat_set_flags"),
  fd_prestat_dir_name: unimplemented("fd_prestat_dir_name"),
  fd_prestat_get: unimplemented("fd_prestat_get"),
  fd_read: unimplemented("fd_read"),
  fd_seek: unimplemented("fd_seek"),
  fd_write: unimplemented("fd_write"),
  path_open: unimplemented("path_open"),
  proc_exit: unimplemented("proc_exit"),
};
