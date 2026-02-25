## Summary

Describe what this PR changes.

## Hardware Release Checklist
- [ ] Added hardware board has horizontal spacing between pins divisible by 2.54 (if applicable)
- [ ] New hardware project files are added under the appropriate board/family directory.
- [ ] Table in devboard's readme is updated and reflects components on the board
- [ ] Packaging workflow is updated in [.github/workflows/package-kicad-projects.yml](.github/workflows/package-kicad-projects.yml) with a new `zip -r` line for the new project.
- [ ] Changelog includes a new section describing what hardware was added.


## Next steps: How to release files for manufacturing?
Once all in master is ready, then:
- Create tag in ISO 8601 format (`YYYY-MM-DD`, for example `2026-03-01`) from the `master` commit
- push it, github action will be triggered, data will be packed into zip
- Verify the GitHub Release for that tag contains the expected ZIP assets.