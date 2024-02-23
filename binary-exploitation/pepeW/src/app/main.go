package main

import (
	"bytes"
	"context"
	"errors"
	"fmt"
	"go/parser"
	"go/token"
	"math/rand"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"time"
)

func main() {
	var err error

	if len(os.Args) < 2 {
		fmt.Printf("Usage: %s <file>", os.Args[0])
		return
	}

	path := os.Args[1]
	file, err := os.OpenFile(path, os.O_RDONLY, os.ModePerm)
	if err != nil {
		fmt.Printf("Failed to open file %s: %v", path, err)
		return
	}
	defer file.Close()

	data, err := os.ReadFile(path)
	if err != nil {
		fmt.Println("Failed to read file")
		return
	}

	lines := strings.Split(string(data), "\n")
	if len(lines) > 150 {
		fmt.Println("File too large")
		return
	}

	buffer := make([]byte, 1024)
	n, err := file.Read(buffer)
	if err != nil {
		fmt.Printf("Failed to read file %s: %v", path, err)
		return
	}
	file.Close()
	defer os.Remove(path)

	if err = sanitize(string(buffer[:n])); err != nil {
		return
	}

	var out string
	if out, err = run(path); err != nil {
		return
	}

	var buf bytes.Buffer
	buf.WriteString(out)
	fmt.Println(buf.String())

	fmt.Printf("Output: %s\n", out)
}

func sanitize(src string) error {
	fs := token.NewFileSet()
	file, _ := parser.ParseFile(fs, "", src, parser.AllErrors)
	// if err != nil {
	// 	fmt.Printf("Failed to sanitize: %v", err)
	// 	return errors.New("")
	// }

	for _, v := range file.Imports {
		switch v.Path.Value {
		case `"reflect"`:
			continue
		default:
			fmt.Printf("import %s is not allowed", v.Path.Value)
			return errors.New("")
		}
	}

	return nil
}

func run(src string) (string, error) {
	dir, err := os.Getwd()
	if err != nil {
		fmt.Printf("Failed to get executable: %v", err)
		return "", errors.New("")
	}

	tmp := filepath.Join(dir, "tmp")
	binary := RandomUpto8Char()
	binary = filepath.Join(tmp, binary+".exe")

	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()

	cmd := exec.CommandContext(ctx, "go", "build", "-o", binary, src)
	cmd.Dir = dir
	defer os.Remove(binary)
	if err := cmd.Run(); err != nil {
		if ctx.Err() == context.DeadlineExceeded {
			fmt.Printf("Run timed out: %v", err)
		} else {
			fmt.Printf("Build failed: %v", err)
		}

		return "", errors.New("")
	}

	out, err := exec.CommandContext(ctx, binary).CombinedOutput()
	if err != nil {
		if ctx.Err() == context.DeadlineExceeded {
			fmt.Printf("Run timed out: %v", err)
		} else {
			fmt.Printf("Execute failed: %v", err)
		}

		return "", errors.New("")
	}

	return string(out), nil
}

const letter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

func RandomUpto8Char() string {
	var len int32 = 0

	len = rand.Int31() % 8

	b := make([]byte, len)
	for i := range b {
		b[i] = letter[rand.Int31()%len]
	}
	return string(b)
}
