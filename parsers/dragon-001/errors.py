#!/usr/bin/env python
#
# Copyright 2012 Google Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Contains the application errors."""


class Error(Exception):
  """Base error type."""

  def __init__(self, error_message):
    self.error_message = error_message


class NotFoundError(Error):
  """Raised when necessary entities are missing."""


class OperationFailedError(Error):
  """Raised when necessary operation has failed."""
