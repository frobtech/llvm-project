"""
Test lldb-dap memory support
"""

from base64 import b64decode
import dap_server
from lldbsuite.test.decorators import *
from lldbsuite.test.lldbtest import *
from lldbsuite.test import lldbutil
import lldbdap_testcase
import os


class TestDAP_memory(lldbdap_testcase.DAPTestCaseBase):
    @skipIfWindows
    def test_memory_refs_variables(self):
        """
        Tests memory references for evaluate
        """
        program = self.getBuildArtifact("a.out")
        self.build_and_launch(program)
        source = "main.cpp"
        self.source_path = os.path.join(os.getcwd(), source)
        self.set_source_breakpoints(
            source,
            [line_number(source, "// Breakpoint")],
        )
        self.continue_to_next_stop()

        locals = {l["name"]: l for l in self.dap_server.get_local_variables()}

        # Pointers should have memory-references
        self.assertIn("memoryReference", locals["rawptr"].keys())
        # Non-pointers should also have memory-references
        self.assertIn("memoryReference", locals["not_a_ptr"].keys())

    @skipIfWindows
    def test_memory_refs_evaluate(self):
        """
        Tests memory references for evaluate
        """
        program = self.getBuildArtifact("a.out")
        self.build_and_launch(program)
        source = "main.cpp"
        self.source_path = os.path.join(os.getcwd(), source)
        self.set_source_breakpoints(
            source,
            [line_number(source, "// Breakpoint")],
        )
        self.continue_to_next_stop()

        self.assertIn(
            "memoryReference",
            self.dap_server.request_evaluate("rawptr")["body"].keys(),
        )

    @skipIfWindows
    def test_memory_refs_set_variable(self):
        """
        Tests memory references for `setVariable`
        """
        program = self.getBuildArtifact("a.out")
        self.build_and_launch(program)
        source = "main.cpp"
        self.source_path = os.path.join(os.getcwd(), source)
        self.set_source_breakpoints(
            source,
            [line_number(source, "// Breakpoint")],
        )
        self.continue_to_next_stop()

        ptr_value = self.get_local_as_int("rawptr")
        self.assertIn(
            "memoryReference",
            self.dap_server.request_setVariable(1, "rawptr", ptr_value + 2)[
                "body"
            ].keys(),
        )

    @skipIfWindows
    def test_readMemory(self):
        """
        Tests the 'readMemory' request
        """
        program = self.getBuildArtifact("a.out")
        self.build_and_launch(program)
        source = "main.cpp"
        self.source_path = os.path.join(os.getcwd(), source)
        self.set_source_breakpoints(
            source,
            [line_number(source, "// Breakpoint")],
        )
        self.continue_to_next_stop()

        ptr_deref = self.dap_server.request_evaluate("*rawptr")["body"]
        memref = ptr_deref["memoryReference"]

        # We can read the complete string
        mem = self.dap_server.request_readMemory(memref, 0, 5)["body"]
        self.assertEqual(b64decode(mem["data"]), b"dead\0")

        # We can read large chunks, potentially returning partial results
        mem = self.dap_server.request_readMemory(memref, 0, 4096)["body"]
        self.assertEqual(b64decode(mem["data"])[0:5], b"dead\0")

        # Use an offset
        mem = self.dap_server.request_readMemory(memref, 2, 3)["body"]
        self.assertEqual(b64decode(mem["data"]), b"ad\0")

        # Reads of size 0 are successful
        # VS Code sends those in order to check if a `memoryReference` can actually be dereferenced.
        mem = self.dap_server.request_readMemory(memref, 0, 0)
        self.assertEqual(mem["success"], True)
        self.assertNotIn(
            "data", mem["body"], f"expects no data key in response: {mem!r}"
        )

        # Reads at offset 0x0 return unreadable bytes
        bytes_to_read = 6
        mem = self.dap_server.request_readMemory("0x0", 0, bytes_to_read)
        self.assertEqual(mem["body"]["unreadableBytes"], bytes_to_read)

        # Reads with invalid address fails.
        mem = self.dap_server.request_readMemory("-3204", 0, 10)
        self.assertFalse(mem["success"], "expect fail on reading memory.")

        self.continue_to_exit()
